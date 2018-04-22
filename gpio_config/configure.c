#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libconfig.h>

config_t cfg;

size_t write_file(char *filepath, unsigned char *data, unsigned int data_length) {
    size_t bytes_written;
    char* mode = "wb";

    FILE *f = fopen(filepath, mode);
    if (f == NULL) {
        fprintf(stderr, "Error: unable to open file %s\n", filepath);
        return 0;
    }
    bytes_written = fwrite(data, 1, data_length, f);

    fclose(f);
    return bytes_written;
}

void getvalue(const char *lookupStr, unsigned int *value) {
	if(config_lookup_int(&cfg, lookupStr, value)) {
        //printf("%s address: %u\n", lookupStr, *value);

		//snprintf(address, 9, "%X", *value);

		// printf("Number To Convert: %u \n", *value);
		// printf("Expected hex number: %X \n", *value);
		// printf("Actual hex number: %s \n", address);
    }
    else
        fprintf(stderr, "No %s setting in configuration file.\n", lookupStr);
}

int main(int argc, const char **argv) {
	config_init(&cfg);

	/* Read the file. If there is an error, report it and exit. */
	if(! config_read_file(&cfg, "my_gpio.cfg")) {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
	    config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        return(EXIT_FAILURE);
	}

    const char *pc_ip = "PC_IP";
    const char *pc_ip_val = "";
    if (config_lookup_string(&cfg, pc_ip, &pc_ip_val)) {

    } else {
        fprintf(stderr, "No %s setting in configuration file.\n", pc_ip);
    }

    char *ptr;
    long ret;

    ret = strtoul(pc_ip_val, &ptr, 10);

	const char *pin_0_num = "PIN_0_NUM";
	unsigned int pin_0_num_val;
    getvalue(pin_0_num, &pin_0_num_val);

    const char *pin_1_num = "PIN_1_NUM";
    unsigned int pin_1_num_val;
    getvalue(pin_1_num, &pin_1_num_val);

    const char *pin_2_num = "PIN_2_NUM";
    unsigned int pin_2_num_val;
    getvalue(pin_2_num, &pin_2_num_val);

    char headerFile[1024];
    sprintf(headerFile, "#define PIN_0_NUM %d\n#define PIN_1_NUM %d\n#define PIN_2_NUM %d\n#define PC_IP %lu\n",
      pin_0_num_val, pin_1_num_val, pin_2_num_val, ret);

    const char *FIFODATA = "FIFODATA";
    unsigned int FIFODATA_val;
    getvalue(FIFODATA, &FIFODATA_val);

    const char *FIFOCOUNT = "FIFOCOUNT";
    unsigned int FIFOCOUNT_val;
    getvalue(FIFOCOUNT, &FIFOCOUNT_val);

    const char *STEPCONFIG = "STEPCONFIG";
    unsigned int STEPCONFIG_val;
    getvalue(STEPCONFIG, &STEPCONFIG_val);    

    const char *STEPDELAY = "STEPDELAY";
    unsigned int STEPDELAY_val;
    getvalue(STEPDELAY, &STEPDELAY_val);

    const char *CM_WKUP_ADC_TSC_CLKCTRL = "CM_WKUP_ADC_TSC_CLKCTRL";
    unsigned int CM_WKUP_ADC_TSC_CLKCTRL_val;
    getvalue(CM_WKUP_ADC_TSC_CLKCTRL, &CM_WKUP_ADC_TSC_CLKCTRL_val);

    const char *CTRL = "CTRL";
    unsigned int CTRL_val;
    getvalue(CTRL, &CTRL_val); 

    const char *STEPENABLE = "STEPENABLE";
    unsigned int STEPENABLE_val;
    getvalue(STEPENABLE, &STEPENABLE_val); 


    char headerFile1[2048];
    sprintf(headerFile1, "#define FIFODATA %d\n#define FIFOCOUNT %d\n#define STEPCONFIG %d\n#define STEPDELAY %d\n#define CM_WKUP_ADC_TSC_CLKCTRL %d\n#define CTRL %d\n#define STEPENABLE %d",
      FIFODATA_val, FIFOCOUNT_val, STEPCONFIG_val, STEPDELAY_val, CM_WKUP_ADC_TSC_CLKCTRL_val, CTRL_val, STEPENABLE_val);

    
	write_file("../include/gpio_config.h", headerFile, strlen(headerFile));

    write_file("../include/analog_config.h", headerFile1, strlen(headerFile1));

	printf("gpio_config.h  & analog.h updated\n");

	return 0;
}
