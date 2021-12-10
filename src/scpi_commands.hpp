#ifndef SCPI_COMMANDS_HPP
#define SCPI_COMMANDS_HPP

#define BUFFER_LEN 64

extern volatile uint8_t reset_flag;
extern volatile uint8_t disconnect_flag;

extern uint8_t RESPONSE_LEN;
extern char COMMAND_BUFFER[BUFFER_LEN];
extern char SEND_BUFFER[BUFFER_LEN];

//extern gauge main_chamber;
extern gauge transfer_chamber;
extern gauge turbo;
extern gauge sample_chamber;
//extern gauge pre_pump;

extern blockable_valve valve1;
extern obstructable_valve valve2;
extern angle_valve valve3;
extern angle_valve valve4;

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
#endif
