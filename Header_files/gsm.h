#ifndef GSM_H
#define GSM_H
void gsm_init(void);
void gsm_cmd(char *cmd);
void send_sms(char *num,char *msg);
void receive_sms(char *msg);
void delete_sms(void);
#endif
