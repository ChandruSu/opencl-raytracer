
__kernel void draw(
    __write_only image2d_t img,
    unsigned int width,
    unsigned int height
  )
{
  int x = get_global_id(0);
  int y = get_global_id(1);

  if (x < width && y < height) {
    int2 coord = (int2)(x, y);
    float4 color = (float4)((float)coord.x / width, 0.0f, (float)coord.y / height, 1.0f);
    write_imagef(img, coord, color);
  }
}