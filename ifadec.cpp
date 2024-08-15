#include <fadec.h>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

char out[2048] = {0};
uint8_t opbuffer[128] = { 0 };

extern "C" __declspec(dllexport) const char* WINAPI DecodeOpcode(const char* opcode, uint16_t size, uint32_t address) {
	memcpy(opbuffer, opcode, size);
	const uint8_t* p = opbuffer;
	string buffer;
	size_t nowsize = size;
	size_t nowaddress = address;

	while (nowsize > 1) {
		FdInstr trans;
		size_t cur = fd_decode(p, nowsize, 32, nowaddress, &trans);
		char data[128] = { 0 };
		fd_format(&trans, data, sizeof(data));
		buffer.append(data);
		buffer += '\n';
		nowsize -= cur;
		p += cur;
		nowaddress += cur;
	}

	string original = " ptr ";
	string replaced = " ";

	for (int first = buffer.find(original); first != -1; first = buffer.find(original)) {
		buffer = buffer.replace(first, original.length(), replaced);
	}

	memset(opbuffer, 0, sizeof(opbuffer));
	_strnset_s(out, 0, 2048);
	strcpy_s(out, buffer.c_str());
	return out;
}
