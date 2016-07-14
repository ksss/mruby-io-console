# mruby-io-console

The implementation of io/console as CRuby.

# Implemented API

- `IO#raw`
- `IO#raw!`
- `IO#cooked`
- `IO#cooked!`
- `IO#winsize`

# Installation

## use github repository

build_config.rb

```ruby
MRuby::Build.new do |conf|
  # by mgem
  conf.gem :mgem => 'mruby-io-console'
  # by github
  conf.gem :github => 'ksss/mruby-io-console', :branch => 'master'
end
```

# Homepage

https://github.com/ksss/mruby-io-console

# License

MIT

# Respect

https://github.com/ruby/ruby/tree/trunk/ext/io/console

# See also

http://ruby-doc.org/stdlib-2.3.0/libdoc/io/console/rdoc/IO.html
