#include "common/serializer.h"

namespace bustub {

template <>
string Deserializer::Read() {
	uint32_t size = Read<uint32_t>();
	if (size == 0) {
		return {};
	}
	auto buffer = unique_ptr<data_t[]>(new data_t[size]);
	ReadData(buffer.get(), size);
	return string(reinterpret_cast<char *>(buffer.get()), size);
}

void Deserializer::ReadStringVector(vector<string> &list) {
	uint32_t sz = Read<uint32_t>();
	list.resize(sz);
	for (int64_t i = 0; i < sz; i++) {
		list[i] = Read<string>();
	}
}

} // namespace bustub

