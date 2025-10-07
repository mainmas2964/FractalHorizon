#ifndef CHUNK_H
#define CHUNK_H
#define CHUNK_X 64
#define CHUNK_Y 64
#define CHUNK_Z 64
class Chunk {
public:
    Chunk();
    ~Chunk();
    bool isLoaded() const;
    void setLoaded(bool loaded);
    void load();
    void unload();
private:
    bool _isLoaded;

};
#endif