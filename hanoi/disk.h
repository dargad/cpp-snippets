#ifndef DISK_H
#define DISK_H

class Disk
{
public:
    Disk(): m_size(-1) {};
    Disk(int size_) : m_size(size_) {};
    virtual ~Disk() { };

    int size() const;

private:
    int m_size;
};

inline
int Disk::size() const
{
    return m_size;
}

#endif /* DISK_H */
