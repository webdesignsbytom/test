#include <stdio.h>

typedef struct
{
    double steps_per_mm;
} MachineConfig;

typedef struct
{
    double current_position[3];
    int pen_down; // 1 if the pen is down, 0 if it's up
} MachineState;

void machine_init(MachineConfig *config, MachineState *state)
{
    config->steps_per_mm = 100.0;
    state->current_position[0] = 0.0;
    state->current_position[1] = 0.0;
    state->current_position[2] = 0.0;
    state->pen_down = 0; // Pen starts in the up position
}

void machine_move(MachineState *state, double x, double y, double z)
{
    state->current_position[0] = x;
    state->current_position[1] = y;
    state->current_position[2] = z;
    // Add code here to actually move the machine
}

void machine_tap(MachineState *state)
{
    // Set the pen_down field to 1 to indicate that the finger is tapping
    state->pen_down = 1;

    // Add code here to actually tap the finger
    // This will depend on the specifics of your machine and how it's controlled

    // Set the pen_down field back to 0 to indicate that the finger has returned to its home position
    state->pen_down = 0;
}

// Function to execute a GPGL command
void machine_execute_command(MachineConfig *config, MachineState *state, const char *command)
{
    // Copy the command so we can modify it
    char *command_copy = strdup(command);

    // Tokenize the command
    char *token = strtok(command_copy, " ");
    while (token != NULL)
    {
        if (strncmp(token, "P1", 2) == 0)
        {
            // Handle pen down command
            state->pen_down = 1;
        }
        else if (strncmp(token, "P0", 2) == 0)
        {
            // Handle pen up command
            state->pen_down = 0;
        }
        else if (token[0] == 'X')
        {
            // Handle X coordinate
            state->current_position[0] = atof(token + 1);
        }
        else if (token[0] == 'Y')
        {
            // Handle Y coordinate
            state->current_position[1] = atof(token + 1);
        }
        else if (token[0] == 'Z')
        {
            // Handle Z coordinate (your custom extension)
            state->current_position[2] = atof(token + 1);
        }

        token = strtok(NULL, " ");
    }

    // Free the copied command
    free(command_copy);

    // Add code here to actually move the machine
    machine_move(state, state->current_position[0], state->current_position[1], state->current_position[2]);
}

int main()
{
    MachineConfig config;
    MachineState state;

    // Initialize the machine
    machine_init(&config, &state);

    // Example usage: move the machine to (10, 20, 30) and set the feed rate to 100
    machine_move(&state, 10.0, 20.0, 30.0);
    machine_set_feed_rate(&state, 100.0);

    // Example usage: execute a custom TPGL command
    const char *tpgl_command = "P1 X50 Y50 Z1;";
    machine_execute_command(&config, &state, tpgl_command);

    return 0;
}