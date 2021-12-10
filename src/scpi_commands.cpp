#include <scpiparser.h>
#include <valve.hpp>
#include <gauge.hpp>
#include "scpi_commands.hpp"

// System commands
scpi_error_t identify(struct scpi_parser_context* context, struct scpi_token* command)
{
    scpi_free_tokens(command);
    RESPONSE_LEN = snprintf(SEND_BUFFER, BUFFER_LEN, "OIC,Embedded SCPI Example,1,10\n");
    return SCPI_SUCCESS;
}

scpi_error_t reset(struct scpi_parser_context* context, struct scpi_token* command)
{
    scpi_free_tokens(command);
    RESPONSE_LEN = snprintf(SEND_BUFFER, BUFFER_LEN, "Resetting now...\n");
    reset_flag = 1;
    return SCPI_SUCCESS;
}

scpi_error_t disconnect(struct scpi_parser_context* context, struct scpi_token* command)
{
    scpi_free_tokens(command);
    RESPONSE_LEN = snprintf(SEND_BUFFER, BUFFER_LEN, "Closing connection...\n");
    disconnect_flag = 1;
    return SCPI_SUCCESS;
}
/*
scpi_error_t change_connection(struct scpi_parser_context* context, struct scpi_token* command)
{
    scpi_free_tokens(command);
    RESPONSE_LEN = snprintf(SEND_BUFFER, BUFFER_LEN, "Changing connection...\n");
    change_flag = 1;
    return SCPI_SUCCESS;
}
*/

// Valve1 commands
scpi_error_t valve1_open(struct scpi_parser_context* context, struct scpi_token* command)
{
    valve1.open();
    scpi_free_tokens(command);
    RESPONSE_LEN = 0;
    return SCPI_SUCCESS;
}


scpi_error_t valve1_close(struct scpi_parser_context* context, struct scpi_token* command)
{
    valve1.close();
    scpi_free_tokens(command);
    RESPONSE_LEN = 0;
    return SCPI_SUCCESS;
}


scpi_error_t valve1_getstate(struct scpi_parser_context* context, struct scpi_token* command)
{
    scpi_free_tokens(command);
    RESPONSE_LEN = snprintf(SEND_BUFFER, BUFFER_LEN, "%s\n", VALVE_STATE_STRING[valve1.state]);
    return SCPI_SUCCESS;
}


// Valve2 commands
scpi_error_t valve2_open(struct scpi_parser_context* context, struct scpi_token* command)
{
    valve2.open();
    scpi_free_tokens(command);
    RESPONSE_LEN = 0;
    return SCPI_SUCCESS;
}


scpi_error_t valve2_close(struct scpi_parser_context* context, struct scpi_token* command)
{
    valve2.close();
    scpi_free_tokens(command);
    RESPONSE_LEN = 0;
    return SCPI_SUCCESS;
}


scpi_error_t valve2_getstate(struct scpi_parser_context* context, struct scpi_token* command)
{
    scpi_free_tokens(command);
    RESPONSE_LEN = snprintf(SEND_BUFFER, BUFFER_LEN, "%s\n", VALVE_STATE_STRING[valve2.state]);
    return SCPI_SUCCESS;
}


// Valve3 commands
scpi_error_t valve3_open(struct scpi_parser_context* context, struct scpi_token* command)
{
    valve3.open();
    scpi_free_tokens(command);
    RESPONSE_LEN = 0;
    return SCPI_SUCCESS;
}


scpi_error_t valve3_close(struct scpi_parser_context* context, struct scpi_token* command)
{
    valve3.close();
    scpi_free_tokens(command);
    RESPONSE_LEN = 0;
    return SCPI_SUCCESS;
}


scpi_error_t valve3_getstate(struct scpi_parser_context* context, struct scpi_token* command)
{
    scpi_free_tokens(command);
    RESPONSE_LEN = snprintf(SEND_BUFFER, BUFFER_LEN, "%s\n", VALVE_STATE_STRING[valve3.state]);
    return SCPI_SUCCESS;
}


// Valve4 commands
scpi_error_t valve4_open(struct scpi_parser_context* context, struct scpi_token* command)
{
    valve4.open();
    scpi_free_tokens(command);
    RESPONSE_LEN = 0;
    return SCPI_SUCCESS;
}


scpi_error_t valve4_close(struct scpi_parser_context* context, struct scpi_token* command)
{
    valve4.close();
    scpi_free_tokens(command);
    RESPONSE_LEN = 0;
    return SCPI_SUCCESS;
}


scpi_error_t valve4_getstate(struct scpi_parser_context* context, struct scpi_token* command)
{
    scpi_free_tokens(command);
    RESPONSE_LEN = snprintf(SEND_BUFFER, BUFFER_LEN, "%s\n", VALVE_STATE_STRING[valve4.state]);
    return SCPI_SUCCESS;
}
