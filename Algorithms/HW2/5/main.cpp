#include <fstream>
#include <vector>


// где:
typedef unsigned char byte;
#define interface struct

interface IInputStream {
    // Возвращает false, если поток закончился
    virtual bool Read(byte& value) = 0;
};

interface IOutputStream {
    virtual void Write(byte value) = 0;
};


class FileInputStream : public IInputStream {
public:
    explicit FileInputStream(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        buffer = std::vector<byte>(std::istreambuf_iterator<char>(file), {});
        position = 0;
    }

    bool Read(byte& value) override {
        if (position < buffer.size()) {
            value = buffer[position++];
            return true;
        } else {
            return false;
        }
    }

private:
    std::vector<byte> buffer;
    size_t position;
};

class FileOutputStream : public IOutputStream {
public:
    explicit FileOutputStream(const std::string& filename) {
        file.open(filename, std::ios::binary);
    }

    virtual void Write(byte value) override {
        if (file.is_open()) {
            file.write(reinterpret_cast<const char*>(&value), sizeof(byte));
        }
    }

    ~FileOutputStream() {
        if (file.is_open()) {
            file.close();
        }
    }

private:
    std::ofstream file;
};


// Метод архивирует данные из потока original
void Encode(IInputStream& original, IOutputStream& compressed);
// Метод восстанавливает оригинальные данные
void Decode(IInputStream& compressed, IOutputStream& original);

int main() {
    FileInputStream inputStream("input.txt");
    FileOutputStream outputStream("output.txt");

    byte value;
    while (inputStream.Read(value)) {
        outputStream.Write(value);
    }

    return 0;
}