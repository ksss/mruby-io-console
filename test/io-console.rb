assert 'IO#raw!' do
  assert_equal true, $stdin.respond_to?(:raw!)
end

assert 'IO#cooked!' do
  assert_equal true, $stdin.respond_to?(:cooked!)
end

assert 'IO#raw' do
  # TODO: fix mruby
  # assert_raise(LocalJumpError) { $stdin.raw }
  $stdin.raw do |io|
    assert_equal $stdin, io
  end
  assert_raise(StandardError) do
    $stdin.raw do |io|
      raise StandardError
    end
  end
end

assert 'IO#cooked' do
  # TODO: fix mruby
  # assert_raise(LocalJumpError) { $stdin.cooked }
  $stdin.cooked do |io|
    assert_equal $stdin, io
  end
  assert_raise(StandardError) do
    $stdin.cooked do |io|
      raise StandardError
    end
  end
end

assert 'IO#winsize' do
  winsize = $stdin.winsize
  assert_kind_of Array, winsize
  assert_equal 2, winsize.length
  assert_equal true, winsize.all? { |i| 0 <= i }
end
