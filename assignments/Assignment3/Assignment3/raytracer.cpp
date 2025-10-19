#include <iostream>
#include <cmath>
#include <limits>

// Vec3
class Vec3 {
public:
    double x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(double t) const { return Vec3(x * t, y * t, z * t); }
    Vec3 operator/(double t) const { return *this * (1 / t); }

    double dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3 normalize() const {
        double len = std::sqrt(x * x + y * y + z * z);
        return *this / len;
    }
};

using Color = Vec3;
using Point3 = Vec3;

// Ray
class Ray {
public:
    Point3 orig;
    Vec3 dir;

    Ray(const Point3& origin, const Vec3& direction) : orig(origin), dir(direction) {}

    Point3 at(double t) const {
        return orig + dir * t;
    }
};

// Sphere
class Sphere {
public:
    Point3 center;
    double radius;
    Color color;

    Sphere(const Point3& c, double r, const Color& col) : center(c), radius(r), color(col) {}

    bool hit(const Ray& r, double& t) const {
        Vec3 oc = r.orig - center;
        auto a = r.dir.dot(r.dir);
        auto b = 2.0 * oc.dot(r.dir);
        auto c = oc.dot(oc) - radius * radius;
        auto discriminant = b * b - 4 * a * c;
        if (discriminant < 0) return false;
        t = (-b - std::sqrt(discriminant)) / (2.0 * a);
        return (t > 0);
    }
};

// Write out colors to stream
void write_color(std::ostream& out, const Color& pixel_color) {
    int ir = static_cast<int>(255.999 * pixel_color.x);
    int ig = static_cast<int>(255.999 * pixel_color.y);
    int ib = static_cast<int>(255.999 * pixel_color.z);
    out << ir << ' ' << ig << ' ' << ib << '\n';
}

// Ray color
Color ray_color(const Ray& r, const Sphere spheres[], int sphere_count) {
    double t_closest = std::numeric_limits<double>::max();
    const Sphere* hit_sphere = nullptr;

    // Find closest sphere hit
    for (int i = 0; i < sphere_count; i++) {
        double t;
        if (spheres[i].hit(r, t) && t < t_closest) {
            t_closest = t;
            hit_sphere = &spheres[i];
        }
    }

    if (hit_sphere) {
        Point3 hit_point = r.at(t_closest);
        Vec3 normal = (hit_point - hit_sphere->center).normalize();
        Vec3 light_dir = Vec3(1, 1, 1).normalize();

        double diffuse = std::max(0.0, normal.dot(light_dir));
        return hit_sphere->color * diffuse;
    }

    // Background
    Vec3 unit_direction = r.dir.normalize();
    double t = 0.5 * (unit_direction.y + 1.0);
    return Color(1.0, 1.0, 1.0) * (1.0 - t) + Color(0.5, 0.7, 1.0) * t;
}

// Main function
int main() {
    // Image
    const int image_width = 800;
    const int image_height = 400;

    // Camera
    Point3 origin(0, 1, 2); // camera position
    double viewport_height = 2.0; // field of view
    double viewport_width = (double)image_width / image_height * viewport_height;
    double focal_length = 1.0; //distance to image plane

    Vec3 horizontal(viewport_width, 0, 0);
    Vec3 vertical(0, viewport_height, 0);
    Point3 lower_left = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);

    // Sphere colors and positions
    Sphere spheres[] = {
        Sphere(Point3(0,-100.5,-1), 100, Color(0.2,0.8,0.0)),
        Sphere(Point3(0,0,-1), 0.5, Color(0.7,0.5,0.3)),
        Sphere(Point3(-3,0,-1.5), 0.5, Color(0.1,0.7,0.9)),
        Sphere(Point3(5,0,-1.5), 0.5, Color(0.8,0.3,0.7))
    };

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::clog << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            double u = double(i) / (image_width - 1);
            double v = double(j) / (image_height - 1);

            Ray r(origin, lower_left + horizontal * u + vertical * v - origin);
            Color pixel_color = ray_color(r, spheres, 4);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\nDone.\n";
}
