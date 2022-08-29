
/* Structs and Constants */
__constant float PI         = 3.14159265359f;
__constant float EPSILON    = 0.00001f;

typedef struct Ray {
  float3 pos;
  float3 dir;
} Ray;

typedef struct RayHit {
  float  dist;
  float3 pos;
  float3 normal;
} RayHit;

typedef struct Sphere {
  float3 center;
  float  radius;
} Sphere;

typedef struct Plane {
  float3 normal;
  float  d;
} Plane;

/* Ray tracing methods. */

Ray createCameraRay(float2 coord, float2 dim) 
{
  float ux = coord.x / dim.x;
  float uy = coord.y / dim.y;
  float aspect = dim.x / dim.y;

  float wx = (ux - 0.5f) * aspect;
  float wy = (uy - 0.5f);

  float3 px = (float3)(wx, -wy, 0.0f);

  Ray r;
  r.pos = (float3)(0.0f, 0.0f, 1.0f);
  r.dir = normalize((float3)(px - r.pos));
  return r;
}

RayHit raySphereIntersect(Ray* ray, Sphere* sphere)
{
  float3 ray2center = sphere->center - ray->pos;
  
	float b = -2.0f * dot(ray2center, ray->dir);
	float c = dot(ray2center, ray2center) - sphere->radius*sphere->radius;

  float discriminant = b * b - 4 * c;

  RayHit hit;
  hit.dist = 0.0f;
  hit.pos = (float3)(0.0f, 0.0f, 0.0f);
  hit.normal = (float3)(0.0f, 0.0f, 0.0f);

  if (discriminant < 0.0f) {
    return hit;
  }

  discriminant = sqrt(discriminant);
  float lambda   = min((-b + discriminant) / 2, (-b - discriminant) / 2);

  if (lambda > EPSILON) {
    hit.dist = lambda;
    hit.pos = lambda * ray->dir + ray->pos;
    hit.normal = normalize(hit.pos - sphere->center);
  }

  return hit;
}

RayHit rayPlaneIntersect(Ray* ray, Plane* plane)
{
  float a = plane->d + dot(plane->normal, ray->pos);
  float b = dot(plane->normal, ray->dir);

  RayHit hit;
  hit.dist = 0.0f;
  hit.pos = (float3)(0.0f, 0.0f, 0.0f);
  hit.normal = (float3)(0.0f, 0.0f, 0.0f);

  if (b != 0.0f) {
    hit.dist = a / b;
    hit.pos = hit.dist * ray->dir + ray->pos;
    hit.normal = plane->normal;
  }
  
  return hit;
}

/* Kernel method draws full image.  */

__kernel void trace (
    __write_only image2d_t img,
    unsigned int width,
    unsigned int height
  )
{
  int x = get_global_id(0);
  int y = get_global_id(1);

  if (x < width && y < height) 
  {
    // Scene
    Ray ray = createCameraRay((float2)(x, y), (float2)(width, height));
    
    Sphere sphere;
    sphere.radius = 1.0f;
    sphere.center = (float3)(0.0f, 0.0f, -3.0f);

    Plane plane;
    plane.normal = normalize((float3)(0.0f, 1.0f, 0.0f));
    plane.d = 2.f;

    // intersection
    RayHit hit = raySphereIntersect(&ray, &sphere);
    RayHit hit0 = rayPlaneIntersect(&ray, &plane);

    float3 lightPos = (float3)(-500.0f, 1000.0f, -700.0f);
    // float3 lightPos = (float3)(0.0f, 2.0f, 1.0f);

    float4 color = (float4)(0.0f, 0.0f, 0.0f, 1.0f);

    if (hit.dist > 0.0f && (hit0.dist <= 0.0f || hit.dist < hit0.dist)) {
      color = (float4)(1.0f, 0.0f, 0.0f, 1.0f) * max(0.10f + dot(normalize(hit.pos - lightPos), hit.normal) / 2.0f, 0.05f);
    } else if (hit0.dist > 0.0f) {
      Ray ray0;
      ray0.pos = hit0.pos;
      ray0.dir = -normalize(lightPos - hit0.pos);
      
      float lighting = max(0.5f + 0.5f * dot(normalize(lightPos - hit0.pos), hit0.normal), 0.05f);
      RayHit hit1 = raySphereIntersect(&ray0, &sphere);

      if (hit1.dist > 0.0f) {
        lighting = 0.15f;
      }

      color = (float4)(0.96f, 0.31f, 0.21f, 1.0f) * lighting;
    }

    write_imagef(img, (int2)(x, y), color);
  }
}