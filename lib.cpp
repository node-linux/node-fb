#include <iostream>

#include <napi.h>

#include <fcntl.h>
#include <linux/fb.h>

#include <sys/ioctl.h>
#include <sys/mman.h>

Napi::Value open_fb(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Invalid arguments. Expected (string)").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string fb_dev = info[0].As<Napi::String>().Utf8Value();

    int fb = open(fb_dev.c_str(), O_RDWR);

    if (fb < 0) {
        Napi::TypeError::New(env, "Error opening device").ThrowAsJavaScriptException();
        return env.Null();
    }

    struct fb_var_screeninfo vscreen_info;
    int err = ioctl(fb, FBIOGET_VSCREENINFO, &vscreen_info);

    if (err) {
        Napi::TypeError::New(env, "IO Error").ThrowAsJavaScriptException();
        return env.Null();
    }

    size_t size = 4 * vscreen_info.xres * vscreen_info.yres;
    auto *buf = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
    Napi::Buffer<char> data = Napi::Buffer<char>::New(env, (char *)buf, size);

    auto fb_utils = Napi::Object::New(env);

    fb_utils.Set("width", Napi::Number::New(env, vscreen_info.xres));
    fb_utils.Set("height", Napi::Number::New(env, vscreen_info.yres));

    fb_utils.Set("device", info[0]);

    fb_utils.Set("data", data);

    return fb_utils;
}

Napi::Value open_file(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    if (info.Length() != 3 || (!info[0].IsNumber() && !info[0].IsString()) || !info[1].IsNumber() || !info[2].IsNumber()) {
        Napi::TypeError::New(env, "Invalid arguments. Expected (number, number, number)").ThrowAsJavaScriptException();
        return env.Null();
    }

    uint32_t fd;

    if (info[0].IsNumber())
        fd = info[0].As<Napi::Number>().Uint32Value();
    else
        fd = open(info[0].As<Napi::String>().Utf8Value().c_str(), O_RDWR);

    uint32_t width = info[1].As<Napi::Number>().Uint32Value();
    uint32_t height = info[2].As<Napi::Number>().Uint32Value();

    size_t size = 4 * width * height;
    fallocate(fd, 0, 0, size);
    auto *buf = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    Napi::Buffer<char> data = Napi::Buffer<char>::New(env, (char *)buf, size);

    auto file = Napi::Object::New(env);

    file.Set("width", Napi::Number::New(env, width));
    file.Set("height", Napi::Number::New(env, height));

    file.Set("fd", fd);

    if (info[0].IsString())
        file.Set("file", info[0]);

    file.Set("data", data);

    return file;
}

Napi::Object init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "open_fb"), Napi::Function::New(env, open_fb));
    exports.Set(Napi::String::New(env, "open_file"), Napi::Function::New(env, open_file));

    return exports;
}

NODE_API_MODULE(nodefb, init);
