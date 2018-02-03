#include "pwr.h"
#include "rt_io_base.h"

pwr_dImport pwr_BindIoUserMethods(I2C_Adapter);
pwr_dImport pwr_BindIoUserMethods(I2C_PCA9555);

pwr_BindIoUserClasses(User) = {
	pwr_BindIoUserClass(I2C_Adapter),
	pwr_BindIoUserClass(I2C_PCA9555),
	pwr_NullClass
}