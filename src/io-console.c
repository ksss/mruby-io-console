#include <errno.h>
#include <sys/ioctl.h>
#include "mruby.h"
#include "mruby/array.h"
#include "mruby/data.h"
#include "mruby/error.h"
#include "mruby/ext/io.h"

#include <termios.h>
typedef struct termios conmode;

static int
setattr(int fd, conmode *t)
{
  while (tcsetattr(fd, TCSAFLUSH, t)) {
    if (errno != EINTR) return 0;
  }
  return 1;
}
#define getattr(fd, t) (tcgetattr(fd, t) == 0)

typedef struct winsize mrb_console_size_t;
#define getwinsize(fd, buf) (ioctl((fd), TIOCGWINSZ, (buf)) == 0)
#define setwinsize(fd, buf) (ioctl((fd), TIOCSWINSZ, (buf)) == 0)
#define winsize_row(buf) (buf)->ws_row
#define winsize_col(buf) (buf)->ws_col

static void
set_rawmode(conmode *t, void *arg)
{
  cfmakeraw(t);
  t->c_lflag &= ~(ECHOE|ECHOK);
}

static void
set_cookedmode(conmode *t, void *arg)
{
  t->c_iflag |= (BRKINT|ISTRIP|ICRNL|IXON);
  t->c_oflag |= OPOST;
  t->c_lflag |= (ECHO|ECHOE|ECHOK|ECHONL|ICANON|ISIG|IEXTEN);
}

static mrb_value
console_yield(mrb_state *mrb, mrb_value block)
{
  return mrb_yield_argv(mrb, block, 1, &mrb->c->stack[0]);
}

static mrb_value
console_raw(mrb_state *mrb, mrb_value self)
{
  mrb_value block;
  mrb_value result;
  mrb_bool state;
  conmode t, bt;

  mrb_get_args(mrb, "&", &block);
  int fd = (int)mrb_fixnum(mrb_io_fileno(mrb, self));
  if (!getattr(fd, &t)) mrb_sys_fail(mrb, 0);
  bt = t;
  set_rawmode(&t, NULL);
  if (!setattr(fd, &t)) mrb_sys_fail(mrb, 0);
  result = mrb_protect(mrb, console_yield, block, &state);
  if (state) {
    mrb_exc_raise(mrb, result);
  }
  return self;
}

static mrb_value
console_set_raw(mrb_state *mrb, mrb_value self)
{
  conmode t;
  int fd = (int)mrb_fixnum(mrb_io_fileno(mrb, self));
  if (!getattr(fd, &t)) mrb_sys_fail(mrb, 0);
  set_rawmode(&t, NULL);
  if (!setattr(fd, &t)) mrb_sys_fail(mrb, 0);
  return self;
}

static mrb_value
console_cooked(mrb_state *mrb, mrb_value self)
{
  mrb_value block;
  mrb_value result;
  mrb_bool state;
  conmode t, bt;

  mrb_get_args(mrb, "&", &block);
  int fd = (int)mrb_fixnum(mrb_io_fileno(mrb, self));
  if (!getattr(fd, &t)) mrb_sys_fail(mrb, 0);
  bt = t;
  set_cookedmode(&t, NULL);
  if (!setattr(fd, &t)) mrb_sys_fail(mrb, 0);
  result = mrb_protect(mrb, console_yield, block, &state);
  if (state) {
    mrb_exc_raise(mrb, result);
  }
  return self;
}

static mrb_value
console_set_cooked(mrb_state *mrb, mrb_value self)
{
  conmode t;
  int fd = (int)mrb_fixnum(mrb_io_fileno(mrb, self));
  if (!getattr(fd, &t)) mrb_sys_fail(mrb, 0);
  set_cookedmode(&t, NULL);
  if (!setattr(fd, &t)) mrb_sys_fail(mrb, 0);
  return self;
}

static mrb_value
console_winsize(mrb_state *mrb, mrb_value self)
{
  mrb_console_size_t ws;

  int fd = (int)mrb_fixnum(mrb_io_fileno(mrb, self));
  if (!getwinsize(fd, &ws)) mrb_sys_fail(mrb, 0);
  return mrb_assoc_new(mrb, mrb_fixnum_value(winsize_row(&ws)), mrb_fixnum_value(winsize_col(&ws)));
}

void
mrb_mruby_io_console_gem_init(mrb_state* mrb)
{
  struct RClass *io = mrb_define_class(mrb, "IO", mrb->object_class);
  mrb_define_method(mrb, io, "raw", console_raw, MRB_ARGS_NONE());
  mrb_define_method(mrb, io, "raw!", console_set_raw, MRB_ARGS_NONE());
  mrb_define_method(mrb, io, "cooked", console_cooked, MRB_ARGS_NONE());
  mrb_define_method(mrb, io, "cooked!", console_set_cooked, MRB_ARGS_NONE());
  mrb_define_method(mrb, io, "winsize", console_winsize, MRB_ARGS_NONE());
}

void
mrb_mruby_io_console_gem_final(mrb_state* mrb)
{
}
