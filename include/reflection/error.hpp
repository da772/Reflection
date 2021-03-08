#pragma once
namespace refl {

    namespace err {

        class err_hndl {
            public:
                inline err_hndl() {
                    err_str = (char*)calloc(1, sizeof(char));
                    err_len = 1;
                }
                inline ~err_hndl() {
                    delete err_str;
                    err_str = nullptr;
                }
                inline void setErrorStr(const char* str) {
                    bHasError = true;
                    size_t len = strlen(str);
                    if (len+1 != err_len) {
                        err_str = (char*)realloc(err_str, sizeof(char)*(len+1) );
                        err_len = len+1;
                        memcpy(err_str, str, sizeof(char)*err_len);
                        err_str[err_len-1] = '\0';
                    } else {
                        memcpy(err_str, str, sizeof(char)*err_len);
                        err_str[err_len-1] = '\0';
                    }
                }
                inline const char* GetError() { bHasError = false; return err_str; }
                inline bool HasError() const {return bHasError; }
            private:
                bool bHasError = false;
                char* err_str = nullptr; 
                size_t err_len;
        };

    }

}