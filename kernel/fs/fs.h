struct fileRef {
	unsigned int nameLength;
	char* name;
	struct fileDescriptor* address;
};

struct fileDescriptor {
	unsigned int length;
	struct fileDescriptor* parent;
	struct fileRef* selfRef;
	char* data;
};

int ls(void);
int cat(char* name, unsigned int nameLength);
int cd(char* name, unsigned int nameLength);
int close(struct fileDescriptor* toClose);
struct fileDescriptor* open(char* name, unsigned int nameLength);
unsigned int read(struct fileDescriptor* fd, char* buffer, unsigned int count);
unsigned int write(struct fileDescriptor* fd, char* buffer, unsigned int count);
int touch(char* name, unsigned int nameLength);
int mkdir(char* name, unsigned int nameLength);
