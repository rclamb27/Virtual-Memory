#pragma once
#include <string>

#define READ_BUF_SIZ 256

class File{
private:
    FILE* type;
public:
    File() = default;

    File(const char* character)
    {
        this->type = fopen(character, "r");
    }
  
    ~File()
    {
        fclose(this->type);
    }
  
    char operator[](int idx) const
    {
        fseek(this->type, idx, 0);
        char ret = fgetc(this->type);
        return ret;
    }

    int getAddress()
    {
        char line[READ_BUF_SIZ];
        fgets(line, READ_BUF_SIZ, type);
        return std::stoi(line);
    }

    
    int parseValue()
    {
        char line[READ_BUF_SIZ];
        memset(line, 0, sizeof(line));
        fgets(line, READ_BUF_SIZ, type);

        int idx = READ_BUF_SIZ - 1;
        for(; idx >= 0; --idx)
        {
            if(line[idx] == ':')
            {
                ++idx; break;
            }
        }
        return std::stoi((char*)&line[idx]); 
    }
};
