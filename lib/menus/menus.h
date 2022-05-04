#include "crsf_protocol.h"

enum data_type {
    UINT8          = 0,
    INT8           = 1,
    UINT16         = 2,
    INT16          = 3,
    FLOAT          = 8,
    TEXT_SELECTION = 9,
    STRING         = 10,
    FOLDER         = 11,
    INFO           = 12,
    COMMAND        = 13,
    OUT_OF_RANGE   = 127,
};



static void parse_bytes(enum data_type type, char **buffer, char *dest);

class Menu {

    uint8_t hidden;
	char *value;
	char *info;
    uint8_t timeout;
    uint8_t min_value;
    uint8_t count;
    uint8_t default_value;

	public:
        
        uint8_t id;
        char *name;
        uint8_t parent;
        uint8_t p_type;
        uint8_t status;
        uint8_t max_value;
        char *optionsMainMenu[50];

        static void ChangeParam(uint8_t param, uint8_t cmd);

        static void loadMainMenu(char *load);

        char * getMainMenuItem_StatusText();

        void displayInfo();

        void divideValueParam (char *values);

		void getParams(char *buffer,int iid);
};


extern Menu menuItems[];

static void parse_bytes(enum data_type type, char **buffer, char *dest);
