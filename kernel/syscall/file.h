struct fileDescriptor* sys_open(char* name);
int sys_close(struct fileDescriptor* fd);
int sys_touch(char* name);
int sys_mkdir(char* name);
unsigned int sys_read(struct fileDescriptor* fd, char* buffer, 
		unsigned int count);
unsigned int sys_write(struct fileDescriptor* fd, char* buffer, 
		unsigned int count);
int sys_ls(void);
int sys_cat(char* name);
int sys_cd(char* name);
