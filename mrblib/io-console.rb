class IO
  def cooked(&block)
    raise LocalJumpError, "no block given" unless block
    cooked!
    yield self
  ensure
    raw!
  end
end
