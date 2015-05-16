struct processState {
	void* pc;
	void* sp;
	void* lr_tmp; /* Space for local lr storage when transfering*/
	void* r4;
	void* r5;
	void* r6;
	void* r7;
	void* r8;
	void* r9;
	void* r10;
	void* r11;
	void* r12;
};
