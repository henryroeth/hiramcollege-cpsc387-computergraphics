#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <sal.h>
#include <vector>
#include <cmath>
#include <cstdint>
#include <limits>
#include <algorithm>

#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

class Vec3 {
public:
    double x, y, z;

    Vec3() : x(0.0), y(0.0), z(0.0) {}
    Vec3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator-() const { return Vec3(-x, -y, -z); }
    Vec3 operator*(double t) const { return Vec3(x * t, y * t, z * t); }
    Vec3 operator*(const Vec3& v) const { return Vec3(x * v.x, y * v.y, z * v.z); }
    Vec3 operator/(double t) const { return Vec3(x / t, y / t, z / t); }

    Vec3& operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vec3& operator-=(const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }

    double dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3 cross(const Vec3& v) const { return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
    double length() const { return std::sqrt(x * x + y * y + z * z); }
    double length_squared() const { return x * x + y * y + z * z; }

    Vec3 normalize() const {
        double len = length();
        if (len <= 0.0) {
            return Vec3();
        }
        return *this / len;
    }

    Vec3 clamp(double min_val, double max_val) const {
        return Vec3(
            std::fmax(min_val, std::fmin(x, max_val)),
            std::fmax(min_val, std::fmin(y, max_val)),
            std::fmax(min_val, std::fmin(z, max_val)));
    }
};

static inline Vec3 operator*(double t, const Vec3& v) {
    return v * t;
}

static Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - n * (2.0 * v.dot(n));
}

using Color = Vec3;
using Point3 = Vec3;

class Ray {
public:
    Point3 orig;
    Vec3 dir;

    Ray() : orig(), dir(0.0, 0.0, 1.0) {}
    Ray(const Point3& origin, const Vec3& direction) : orig(origin), dir(direction) {}

    Point3 at(double t) const {
        return orig + dir * t;
    }
};

class Sphere;

struct HitInfo {
    double t = 0.0;
    Point3 point;
    Vec3 normal;
    bool frontFace = false;
    const Sphere* sphere = nullptr;

    void setFaceNormal(const Ray& r, const Vec3& outward) {
        frontFace = r.dir.dot(outward) < 0.0;
        normal = frontFace ? outward : -outward;
    }
};

class Sphere {
public:
    Point3 center;
    double radius;
    Color albedo;
    bool checker = false;
    Color secondary;
    double textureScale = 1.0;

    Sphere() : center(), radius(1.0), albedo(1.0, 1.0, 1.0), checker(false), secondary(1.0, 1.0, 1.0), textureScale(1.0)
    {
    }
    Sphere(const Point3& c, double r, const Color& col) : center(c), radius(r), albedo(col), checker(false),
        secondary(1.0, 1.0, 1.0), textureScale(1.0) {
    }

    Color sampleColor(const Point3& p) const {
        if (!checker) {
            return albedo;
        }
        double scaledX = p.x * textureScale;
        double scaledZ = p.z * textureScale;
        int pattern = (static_cast<int>(std::floor(scaledX)) + static_cast<int>(std::floor(scaledZ))) & 1;
        return pattern == 0 ? albedo : secondary;
    }

    bool hit(const Ray& r, double tMin, double tMax, HitInfo& hit) const {
        Vec3 oc = r.orig - center;
        double a = r.dir.dot(r.dir);
        double half_b = oc.dot(r.dir);
        double c = oc.dot(oc) - radius * radius;
        double discriminant = half_b * half_b - a * c;
        if (discriminant < 0.0) {
            return false;
        }
        double sqrtD = std::sqrt(discriminant);

        double root = (-half_b - sqrtD) / a;
        if (root < tMin || root > tMax) {
            root = (-half_b + sqrtD) / a;
            if (root < tMin || root > tMax) {
                return false;
            }
        }

        hit.t = root;
        hit.point = r.at(hit.t);
        Vec3 outwardNormal = (hit.point - center) / radius;
        hit.setFaceNormal(r, outwardNormal);
        hit.sphere = this;
        return true;
    }
};

enum class LightType {
    Directional,
    Point
};

struct Light {
    LightType type = LightType::Directional;
    Vec3 direction = Vec3(0.0, -1.0, 0.0);
    Point3 position = Point3();
    Color color = Color(1.0, 1.0, 1.0);
    double intensity = 1.0;
    bool enabled = true;

    static Light directional(const Vec3& dir, const Color& col, double intens) {
        Light light;
        light.type = LightType::Directional;
        light.direction = dir.normalize();
        light.position = Point3();
        light.color = col;
        light.intensity = intens;
        light.enabled = true;
        return light;
    }

    static Light point(const Point3& pos, const Color& col, double intens) {
        Light light;
        light.type = LightType::Point;
        light.direction = Vec3();
        light.position = pos;
        light.color = col;
        light.intensity = intens;
        light.enabled = true;
        return light;
    }
};

class Camera {
public:
    Point3 position = Point3(0.0, 1.2, 3.5);
    double yaw = 3.14159265358979323846;
    double pitch = -0.1;
    double fovY = 60.0 * (3.14159265358979323846 / 180.0);
    double aspect = 1.0;

    Vec3 forward() const {
        double cosPitch = std::cos(pitch);
        return Vec3(
            std::sin(yaw) * cosPitch,
            std::sin(pitch),
            std::cos(yaw) * cosPitch).normalize();
    }

    Vec3 right() const {
        Vec3 f = forward();
        Vec3 worldUp(0.0, 1.0, 0.0);
        Vec3 r = f.cross(worldUp);
        if (r.length_squared() < 1e-8) {
            return Vec3(1.0, 0.0, 0.0);
        }
        return r.normalize();
    }

    Vec3 up() const {
        Vec3 r = right();
        Vec3 f = forward();
        Vec3 u = r.cross(f);
        if (u.length_squared() < 1e-8) {
            return Vec3(0.0, 1.0, 0.0);
        }
        return u.normalize();
    }

    Ray get_ray(double u, double v) const {
        Vec3 f = forward();
        Vec3 r = right();
        Vec3 uvec = up();
        double tanHalfFov = std::tan(0.5 * fovY);
        Vec3 imageRight = r * (2.0 * tanHalfFov * aspect);
        Vec3 imageUp = uvec * (2.0 * tanHalfFov);
        Vec3 lowerLeft = position + f - imageRight * 0.5 - imageUp * 0.5;
        Vec3 pointOnPlane = lowerLeft + imageRight * u + imageUp * v;
        return Ray(position, (pointOnPlane - position).normalize());
    }
};

struct AppState {
    HWND hwnd = nullptr;
    int renderWidth = 800;
    int renderHeight = 400;
    BITMAPINFO bitmapInfo = {};
    std::vector<std::uint32_t> pixels;

    Camera camera;
    std::vector<Sphere> spheres;
    size_t orbitSphereIndex = static_cast<size_t>(-1);
    Point3 orbitCenter;
    double orbitRadius = 2.0;
    double orbitSpeed = 0.6;
    double animationTime = 0.0;

    std::vector<Light> lights;
    int pointLightIndex = -1;

    bool running = true;
    bool keys[256] = {};
    LONG mouseDeltaX = 0;
    LONG mouseDeltaY = 0;
    double moveSpeed = 3.5;
    double mouseSensitivity = 0.0025;
};

static AppState* g_appState = nullptr;

static Color ray_color(const Ray& r, const std::vector<Sphere>& spheres, const std::vector<Light>& lights) {
    HitInfo closestHit;
    bool hitAnything = false;
    double closest = std::numeric_limits<double>::max();

    for (const Sphere& sphere : spheres) {
        HitInfo hit;
        if (sphere.hit(r, 0.001, closest, hit)) {
            hitAnything = true;
            closest = hit.t;
            closestHit = hit;
        }
    }

    if (!hitAnything) {
        Vec3 unit = r.dir.normalize();
        double t = 0.5 * (unit.y + 1.0);
        return Color(1.0, 1.0, 1.0) * (1.0 - t) + Color(0.5, 0.7, 1.0) * t;
    }

    Color baseColor = closestHit.sphere->sampleColor(closestHit.point);
    Vec3 viewDir = (-r.dir).normalize();
    const double ambientStrength = 0.15;
    const double specularStrength = 0.6;
    const double shininess = 32.0;

    Color lighting = baseColor * ambientStrength;

    for (const Light& light : lights) {
        if (!light.enabled) {
            continue;
        }
        Vec3 lightDir;
        double attenuation = 1.0;
        if (light.type == LightType::Directional) {
            lightDir = (-light.direction).normalize();
        }
        else {
            Vec3 toLight = light.position - closestHit.point;
            double distanceSq = toLight.length_squared();
            lightDir = toLight.normalize();
            double distance = std::sqrt(std::max(distanceSq, 1e-6));
            attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distanceSq);
        }

        double diff = std::max(0.0, closestHit.normal.dot(lightDir));
        Color diffuse = (baseColor * light.color) * (diff * light.intensity * attenuation);

        Vec3 reflectDir = reflect(-lightDir, closestHit.normal);
        double spec = std::pow(std::max(0.0, viewDir.dot(reflectDir)), shininess);
        Color specular = light.color * (specularStrength * spec * light.intensity * attenuation);

        lighting = lighting + diffuse + specular;
    }

    return lighting.clamp(0.0, 1.0);
}

static std::uint32_t toPixel(const Color& c) {
    Color clamped = c.clamp(0.0, 0.999);
    std::uint8_t r = static_cast<std::uint8_t>(clamped.x * 255.0);
    std::uint8_t g = static_cast<std::uint8_t>(clamped.y * 255.0);
    std::uint8_t b = static_cast<std::uint8_t>(clamped.z * 255.0);
    return (static_cast<std::uint32_t>(b)) |
        (static_cast<std::uint32_t>(g) << 8) |
        (static_cast<std::uint32_t>(r) << 16);
}

static void renderScene(AppState& app) {
    app.camera.aspect = static_cast<double>(app.renderWidth) / static_cast<double>(app.renderHeight);

    for (int y = 0; y < app.renderHeight; ++y) {
        double v = 1.0 - static_cast<double>(y) / static_cast<double>(app.renderHeight - 1);
        for (int x = 0; x < app.renderWidth; ++x) {
            double u = static_cast<double>(x) / static_cast<double>(app.renderWidth - 1);
            Ray ray = app.camera.get_ray(u, v);
            Color color = ray_color(ray, app.spheres, app.lights);
            app.pixels[static_cast<size_t>(y) * static_cast<size_t>(app.renderWidth)
                + static_cast<size_t>(x)] = toPixel(color);
        }
    }
}

static void presentFrame(AppState& app) {
    if (!app.hwnd) {
        return;
    }

    RECT rect{};
    GetClientRect(app.hwnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    HDC dc = GetDC(app.hwnd);
    StretchDIBits(
        dc,
        0,
        0,
        width,
        height,
        0,
        0,
        app.renderWidth,
        app.renderHeight,
        app.pixels.data(),
        &app.bitmapInfo,
        DIB_RGB_COLORS,
        SRCCOPY);
    ReleaseDC(app.hwnd, dc);
}

static void updateCamera(AppState& app, double dt) {
    Camera& cam = app.camera;
    cam.yaw += static_cast<double>(app.mouseDeltaX) * app.mouseSensitivity;
    cam.pitch -= static_cast<double>(app.mouseDeltaY) * app.mouseSensitivity;
    app.mouseDeltaX = 0;
    app.mouseDeltaY = 0;

    const double pitchLimit = (3.14159265358979323846 / 2.0) - 0.01;
    cam.pitch = std::max(-pitchLimit, std::min(pitchLimit, cam.pitch));

    Vec3 forward = cam.forward();
    Vec3 forwardFlat(forward.x, 0.0, forward.z);
    if (forwardFlat.length_squared() > 1e-6) {
        forwardFlat = forwardFlat.normalize();
    }

    Vec3 worldUp(0.0, 1.0, 0.0);
    Vec3 right = forwardFlat.cross(worldUp);
    if (right.length_squared() > 1e-6) {
        right = right.normalize();
    }

    Vec3 velocity;
    if (app.keys['W']) velocity += forwardFlat;
    if (app.keys['S']) velocity -= forwardFlat;
    if (app.keys['A']) velocity -= right;
    if (app.keys['D']) velocity += right;
    if (app.keys[VK_SPACE]) velocity += worldUp;
    if (app.keys[VK_CONTROL]) velocity -= worldUp;

    if (velocity.length_squared() > 0.0) {
        velocity = velocity.normalize();
        double speed = app.moveSpeed;
        if (app.keys[VK_SHIFT]) {
            speed *= 1.8;
        }
        cam.position += velocity * (speed * dt);
    }
}

static void updateScene(AppState& app, double dt) {
    app.animationTime += dt;

    if (app.orbitSphereIndex < app.spheres.size()) {
        Sphere& orbitSphere = app.spheres[app.orbitSphereIndex];
        double angle = app.animationTime * app.orbitSpeed;
        double bounce = std::sin(app.animationTime * 2.0) * 0.2;
        orbitSphere.center.x = app.orbitCenter.x + std::cos(angle) * app.orbitRadius;
        orbitSphere.center.z = app.orbitCenter.z + std::sin(angle) * app.orbitRadius;
        orbitSphere.center.y = app.orbitCenter.y + bounce;
    }

    if (app.pointLightIndex >= 0 && app.pointLightIndex < static_cast<int>(app.lights.size())) {
        Light& pointLight = app.lights[app.pointLightIndex];
        double angle = app.animationTime * 0.5;
        pointLight.position.x = std::cos(angle) * 3.0;
        pointLight.position.z = std::sin(angle) * 3.0;
        pointLight.position.y = 2.5 + std::sin(app.animationTime * 1.3) * 0.5;
    }
}

static void initializeScene(AppState& app) {
    app.animationTime = 0.0;
    app.spheres.clear();

    Sphere ground(Point3(0.0, -100.5, -1.0), 100.0, Color(0.8, 0.8, 0.8));
    ground.checker = true;
    ground.secondary = Color(0.1, 0.1, 0.1);
    ground.textureScale = 0.5;
    app.spheres.push_back(ground);

    Sphere centerpiece(Point3(0.0, 0.6, -2.6), 0.7, Color(0.7, 0.4, 0.3));
    app.spheres.push_back(centerpiece);

    Sphere orbiting(Point3(1.5, 0.3, -2.6), 0.4, Color(0.2, 0.6, 1.0));
    app.orbitSphereIndex = app.spheres.size();
    app.orbitCenter = Point3(0.0, 0.5, -2.6);
    app.orbitRadius = 2.0;
    app.orbitSpeed = 0.6;
    app.spheres.push_back(orbiting);

    Sphere left(Point3(-1.8, 0.3, -1.5), 0.3, Color(0.9, 0.8, 0.2));
    app.spheres.push_back(left);

    Sphere right(Point3(2.0, 0.4, -3.0), 0.6, Color(0.4, 0.8, 0.5));
    app.spheres.push_back(right);

    Sphere magenta(Point3(-2.2, 0.2, -3.2), 0.4, Color(0.8, 0.3, 0.7));
    app.spheres.push_back(magenta);

    app.lights.clear();
    Light directional = Light::directional(Vec3(-1.0, -1.0, -0.5), Color(1.0, 0.95, 0.9), 1.0);
    app.lights.push_back(directional);

    Light point = Light::point(Point3(1.5, 2.5, 0.0), Color(0.6, 0.7, 1.0), 1.6);
    app.pointLightIndex = static_cast<int>(app.lights.size());
    app.lights.push_back(point);
}

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        CREATESTRUCTW* cs = reinterpret_cast<CREATESTRUCTW*>(lParam);
        g_appState = static_cast<AppState*>(cs->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(g_appState));
        SetCapture(hwnd);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SETFOCUS:
        SetCapture(hwnd);
        return 0;
    case WM_KILLFOCUS:
        ReleaseCapture();
        if (g_appState) {
            std::fill(std::begin(g_appState->keys), std::end(g_appState->keys), false);
        }
        return 0;
    case WM_SIZE:
        if (g_appState) {
            InvalidateRect(hwnd, nullptr, FALSE);
        }
        return 0;
    case WM_KEYDOWN: {
        if (!g_appState) {
            break;
        }
        bool isRepeat = (lParam & (1u << 30)) != 0;
        if (wParam < 256) {
            g_appState->keys[wParam] = true;
        }
        if (wParam == VK_ESCAPE && !isRepeat) {
            g_appState->running = false;
            PostQuitMessage(0);
        }
        else if (wParam == 'L' && !isRepeat) {
            if (g_appState->pointLightIndex >= 0 && g_appState->pointLightIndex <
                static_cast<int>(g_appState->lights.size())) {
                Light& pointLight = g_appState->lights[g_appState->pointLightIndex];
                pointLight.enabled = !pointLight.enabled;
            }
        }
        return 0;
    }
    case WM_KEYUP:
        if (g_appState && wParam < 256) {
            g_appState->keys[wParam] = false;
        }
        return 0;
    case WM_INPUT:
        if (g_appState) {
            RAWINPUT raw{};
            UINT size = sizeof(raw);
            if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, &raw, &size, sizeof(RAWINPUTHEADER)) ==
                size) {
                if (raw.header.dwType == RIM_TYPEMOUSE) {
                    g_appState->mouseDeltaX += raw.data.mouse.lLastX;
                    g_appState->mouseDeltaY += raw.data.mouse.lLastY;
                }
            }
        }
        return 0;
    default:
        break;
    }

    return DefWindowProcW(hwnd, message, wParam, lParam);
}

int WINAPI wWinMain(
    _In_     HINSTANCE hInstance,
    _In_opt_ HINSTANCE,
    _In_     PWSTR,
    _In_     int nCmdShow)
{
    AppState app;
    g_appState = &app;

    app.pixels.resize(static_cast<size_t>(app.renderWidth) * static_cast<size_t>(app.renderHeight));
    std::fill(std::begin(app.keys), std::end(app.keys), false);

    app.bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    app.bitmapInfo.bmiHeader.biWidth = app.renderWidth;
    app.bitmapInfo.bmiHeader.biHeight = -app.renderHeight;
    app.bitmapInfo.bmiHeader.biPlanes = 1;
    app.bitmapInfo.bmiHeader.biBitCount = 32;
    app.bitmapInfo.bmiHeader.biCompression = BI_RGB;

    initializeScene(app);

    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = L"Milestone3Window";

    if (!RegisterClassExW(&wc)) {
        return 0;
    }

    DWORD style = WS_OVERLAPPEDWINDOW;
    RECT rect = { 0, 0, app.renderWidth, app.renderHeight };
    AdjustWindowRect(&rect, style, FALSE);

    HWND hwnd = CreateWindowExW(
        0,
        wc.lpszClassName,
        L"Interactive Ray Tracer - Milestone 3",
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        hInstance,
        &app);

    if (!hwnd) {
        return 0;
    }

    app.hwnd = hwnd;

    RAWINPUTDEVICE rid{}; 
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x02;
    rid.dwFlags = RIDEV_INPUTSINK;
    rid.hwndTarget = hwnd;
    RegisterRawInputDevices(&rid, 1, sizeof(rid));

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    LARGE_INTEGER previous;
    QueryPerformanceCounter(&previous);

    MSG msg = {};
    while (app.running) {
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                app.running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        if (!app.running) {
            break;
        }

        LARGE_INTEGER current;
        QueryPerformanceCounter(&current);
        double dt = static_cast<double>(current.QuadPart - previous.QuadPart) / static_cast<double>(frequency.QuadPart);
        previous = current;
        dt = std::min(dt, 0.1);

        updateCamera(app, dt);
        updateScene(app, dt);
        renderScene(app);
        presentFrame(app);
    }

    return static_cast<int>(msg.wParam);
}
