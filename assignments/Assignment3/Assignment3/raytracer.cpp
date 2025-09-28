#include <iostream>
#include <cmath>
#include <limits>
#include <vector>

// ========== Vec3 ==========
class vec3 {
public:
    double x, y, z;
    vec3() : x(0), y(0), z(0) {}
    vec3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

    vec3 operator+(const vec3& v) const { return vec3(x + v.x, y + v.y, z + v.z); }
    vec3 operator-(const vec3& v) const { return vec3(x - v.x, y - v.y, z - v.z); }
    vec3 operator*(double t) const { return vec3(x * t, y * t, z * t); }
    vec3 operator/(double t) const { return vec3(x / t, y / t, z / t); }

    double dot(const vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    double length() const { return std::sqrt(x * x + y * y + z * z); }
    vec3 normalize() const { return *this / length(); }
};

using point3 = vec3;
using color = vec3;

// ========== Ray ==========
class ray {
public:
    point3 orig;
    vec3 dir;

    ray() {}
    ray(const point3& o, const vec3& d) : orig(o), dir(d) {}
    point3 at(double t) const { return orig + dir * t; }
};

// ========== Sphere ==========
struct sphere {
    point3 center;
    double radius;
    color col;

    bool hit(const ray& r, double t_min, double t_max, double& t_hit, vec3& normal, color& out_col) const {
        vec3 oc = r.orig - center;
        double a = r.dir.dot(r.dir);
        double b = 2.0 * oc.dot(r.dir);
        double c = oc.dot(oc) - radius * radius;
        double discriminant = b * b - 4 * a * c;

        if (discriminant < 0) return false;
        double sqrtd = std::sqrt(discriminant);

        // nearest root in range
        double root = (-b - sqrtd) / (2.0 * a);
        if (root < t_min || root > t_max) {
            root = (-b + sqrtd) / (2.0 * a);
            if (root < t_min || root > t_max)
                return false;
        }

        t_hit = root;
        point3 hit_point = r.at(t_hit);
        normal = (hit_point - center) / radius;
        out_col = col;
        return true;
    }
};

// ========== Ray color (Lambertian shading) ==========
color ray_color(const ray& r, const std::vector<sphere>& scene, const vec3& light_dir) {
    double closest = std::numeric_limits<double>::infinity();
    color hit_color(0, 0, 0);
    bool hit_anything = false;

    for (const auto& s : scene) {
        double t;
        vec3 normal;
        color col;
        if (s.hit(r, 0.001, closest, t, normal, col)) {
            hit_anything = true;
            closest = t;
            double diff = std::max(0.0, normal.normalize().dot(light_dir));
            hit_color = col * diff;
        }
    }

    if (hit_anything) return hit_color;

    // background gradient
    vec3 unit_dir = r.dir.normalize();
    double t = 0.5 * (unit_dir.y + 1.0);
    return color(1.0, 1.0, 1.0) * (1.0 - t) + color(0.5, 0.7, 1.0) * t;
}

// ========== Main ==========
int main() {
    // Image
    const int image_width = 800;
    const int image_height = 400;

    // Camera
    point3 origin(0, 0, 0);
    double viewport_height = 2.0;
    double viewport_width = viewport_height * (double)image_width / image_height;
    double focal_length = 1.0;

    vec3 horizontal(viewport_width, 0, 0);
    vec3 vertical(0, viewport_height, 0);
    point3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

    // Scene (3 spheres + ground)
    std::vector<sphere> scene;
    scene.push_back({ point3(0,-1000.5,-1), 1000.0, color(0.8,0.8,0.0) });
    scene.push_back({ point3(0,0,-1), 0.5, color(0.7,0.3,0.3) });
    scene.push_back({ point3(-1,0,-1), 0.5, color(0.3,0.7,0.3) });
    scene.push_back({ point3(1,0,-1), 0.5, color(0.3,0.3,0.7) });

    // Light
    vec3 light_dir = vec3(1, 1, 1).normalize();

    // Output PPM
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            double u = double(i) / (image_width - 1);
            double v = double(j) / (image_height - 1);
            vec3 dir = lower_left_corner + horizontal * u + vertical * v - origin;
            ray r(origin, dir);

            color pixel = ray_color(r, scene, light_dir);

            int ir = static_cast<int>(255.999 * pixel.x);
            int ig = static_cast<int>(255.999 * pixel.y);
            int ib = static_cast<int>(255.999 * pixel.z);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
