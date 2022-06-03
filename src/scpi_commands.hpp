#ifndef SCPI_COMMANDS_HPP
#define SCPI_COMMANDS_HPP

#define BUFFER_LEN 64

extern volatile uint8_t reset_flag;
extern volatile uint8_t disconnect_flag;

extern uint8_t RESPONSE_LEN;
extern char COMMAND_BUFFER[BUFFER_LEN];
extern char SEND_BUFFER[BUFFER_LEN];

//extern gauge lpq;
//extern gauge sample;
//extern gauge optics;
//extern gauge ccd;


extern blockable_valve valve1;
extern blockable_valve valve2;
extern blockable_valve valve3;
extern blockable_valve valve4;

extern sample_holder holder;

scpi_error_t identify(struct scpi_parser_context* context, struct scpi_token* command);
scpi_error_t reset(struct scpi_parser_context* context, struct scpi_token* command);
scpi_error_t disconnect(struct scpi_parser_context* context, struct scpi_token* command);
//scpi_error_t change_connection(struct scpi_parser_context* context, struct scpi_token* command);

scpi_error_t valve1_open(struct scpi_parser_context* context, struct scpi_token* command);
scpi_error_t valve1_close(struct scpi_parser_context* context, struct scpi_token* command);
scpi_error_t valve1_getstate(struct scpi_parser_context* context, struct scpi_token* command);

scpi_error_t valve2_open(struct scpi_parser_context* context, struct scpi_token* command);
scpi_error_t valve2_close(struct scpi_parser_context* context, struct scpi_token* command);
scpi_error_t valve2_getstate(struct scpi_parser_context* context, struct scpi_token* command);

scpi_error_t valve3_open(struct scpi_parser_context* context, struct scpi_token* command);
scpi_error_t valve3_close(struct scpi_parser_context* context, struct scpi_token* command);
scpi_error_t valve3_getstate(struct scpi_parser_context* context, struct scpi_token* command);

scpi_error_t valve4_open(struct scpi_parser_context* context, struct scpi_token* command);
scpi_error_t valve4_close(struct scpi_parser_context* context, struct scpi_token* command);
scpi_error_t valve4_getstate(struct scpi_parser_context* context, struct scpi_token* command);

scpi_error_t sample_holder_getstate(struct scpi_parser_context* context, struct scpi_token* command);

#endif
