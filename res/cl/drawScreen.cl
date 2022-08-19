

__kernel void draw (
    __write_only image2d_t img,
    unsigned int width,
    unsigned int height,
    float time
  )
{
  int x = get_global_id(0);
  int y = get_global_id(1);

  if (x < width && y < height)
  {
    int2 coord = (int2)(x, y);
    float xCoord = (float)((coord.x + (int)(100 * time)) % width) / width;
    float4 color = (float4)(xCoord, 0.0f, (float) coord.y / height, 1.0f);
    write_imagef(img, coord, color);
  }
}
