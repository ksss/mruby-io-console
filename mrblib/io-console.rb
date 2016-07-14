class IO
  def raw(&block)
    raise LocalJumpError, "no block given" unless block
    raw!
    yield self
  ensure
    cooked!
  end

  def cooked(&block)
    raise LocalJumpError, "no block given" unless block
    cooked!
    yield self
  ensure
    raw!
  end
end
