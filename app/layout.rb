class Layout
  attr_accessor :width, :height, :margin

  def initialize width, height
    @width = width
    @height = height
    @margin = 50
  end

  def title_location
    [(@width / 2).to_i, margin]
  end

  def column_1_frame

  end

  def column_2_frame

  end

  def column_3_frame

  end
end
