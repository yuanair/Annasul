module;

module annasul.container;

namespace annasul {

template <typename T, size_t N>
class StaticArray {
   private:
    T data[N];
};
}  // namespace annasul
