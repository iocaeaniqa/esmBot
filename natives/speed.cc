#include <napi.h>
#include <list>
#include <Magick++.h>

using namespace std;
using namespace Magick;

class SpeedWorker : public Napi::AsyncWorker {
 public:
  SpeedWorker(Napi::Function& callback, string in_path, bool slow, string type, int delay)
      : Napi::AsyncWorker(callback), in_path(in_path), slow(slow), type(type), delay(delay) {}
  ~SpeedWorker() {}

  void Execute() {
    list<Image> frames;
    list<Image> blurred;
    readImages(&frames, in_path);
    
    for_each(frames.begin(), frames.end(), animationDelayImage(slow ? delay * 2 : delay / 2));

    writeImages(frames.begin(), frames.end(), &blob);
  }

  void OnOK() {
    Callback().Call({Env().Undefined(), Napi::Buffer<char>::Copy(Env(), (char *)blob.data(), blob.length())});
  }

 private:
  string in_path, type;
  int delay, wordlength, i, n, amount;
  size_t bytes, type_size;
  Blob blob;
  bool slow;
};

Napi::Value Speed(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  string in_path = info[0].As<Napi::String>().Utf8Value();
  bool slow = info[1].As<Napi::Boolean>().Value();
  string type = info[2].As<Napi::String>().Utf8Value();
  int delay = info[3].As<Napi::Number>().Int32Value();
  Napi::Function cb = info[4].As<Napi::Function>();

  SpeedWorker* explodeWorker = new SpeedWorker(cb, in_path, slow, type, delay);
  explodeWorker->Queue();
  return env.Undefined();
}