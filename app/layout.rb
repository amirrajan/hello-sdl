class Layout
  attr_accessor :width, :height, :square

  def initialize width, height
    @width = width
    @height = height
    @square = 40
  end

  def title_location
    [(width / 2).to_i, square]
  end

  def total_rows
    (@height / @square).to_i
  end

  def total_columns
    (@width / @square).to_i
  end

  def column_1_frame
    x = 1 * square
    y = 1 * square
    width = 7 * square
    height = (total_rows - 2) * square
    [x,
     y,
     width,
     height]
  end

  def column_2_frame
    x = 9 * square
    y = 1 * square
    width = 14 * square
    height = (total_rows - 2) * square
    [x,
     y,
     width,
     height]
  end

  def column_3_frame
    x = 24 * square
    y = 1 * square
    width = 7 * square
    height = (total_rows - 2) * square
    [x,
     y,
     width,
     height]
  end
end
