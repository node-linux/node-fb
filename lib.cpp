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
    Napi::Buffer<char> data = Napi::Buffer<char>::New(env, (char*)buf, size);

    auto fb_utils = Napi::Object::New(env);

    fb_utils.Set(Napi::String::New(env, "width"), Napi::Number::New(env, vscreen_info.xres));
    fb_utils.Set(Napi::String::New(env, "height"), Napi::Number::New(env, vscreen_info.yres));

    fb_utils.Set(Napi::String::New(env, "device"), info[0].As<Napi::String>());

    fb_utils.Set(Napi::String::New(env, "data"), data);

    return fb_utils;
}

Napi::Object init(Napi::Env env, Napi::Object exports) {
	exports.Set(Napi::String::New(env, "open"), Napi::Function::New(env, open_fb));

    return exports;
}

NODE_API_MODULE(nodefb, init);
