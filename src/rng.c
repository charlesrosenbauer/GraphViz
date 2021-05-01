#include "defs.h"

#include "stdint.h"


static uint64_t rseed = 3518092371089;
float rflt(){
	rseed   = (rseed * 1349179819789) + 1471890674187;
	int rot =  rseed & 0x3f;
	rseed   = (rseed >> rot) | (rseed << (64-rot));
	int ret = rseed & 0xff;
	return (((float)ret) / 128.0) - 1.0;
}
