#ifndef _CONSOLE_H
#define  _CONSOLE_H

/* ***** PUBLIC METHODS ***** */
int32_t console_read(char *ptr, uint16_t len);

int32_t console_write(const char *ptr, uint16_t len);

#endif /* _CONSOLE_H */
