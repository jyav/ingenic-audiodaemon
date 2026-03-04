#ifndef CONFIG_H
#define CONFIG_H

#include <cJSON.h>

// -----------------------------
// Configuration Handling Functions
// -----------------------------
#ifndef UTILS_H
#define UTILS_H

// Required headers for thread management and data types
#include <pthread.h>
#include <sys/types.h>      // For ssize_t

// Constants for program tagging and frame duration
#define PROG_TAG "AO_IAD"
#define FRAME_DURATION 0.040

/**
 * @brief Represents a connected client with its socket descriptor.
 *
 * This struct is used to manage connected clients in a linked list.
 */
typedef struct ClientNode {
    int sockfd;  // Socket descriptor for the client
    struct ClientNode *next;  // Pointer to the next client node
} ClientNode;

// Head of the linked list that contains all connected clients
extern ClientNode *client_list_head;

// Mutex lock for audio buffer synchronization
extern pthread_mutex_t audio_buffer_lock;

// Condition variable for signaling availability of audio data
extern pthread_cond_t audio_data_cond;

// Global buffer to store audio data
extern unsigned char *audio_buffer;

// Size of the current audio data in the buffer
extern ssize_t audio_buffer_size;

// Socket descriptor of the currently active client
extern int active_client_sock;

/**
 * @brief Creates a new thread.
 *
 * @param thread_id Pointer to the thread ID.
 * @param start_routine Pointer to the function to run in the new thread.
 * @param arg Argument to pass to the start_routine.
 * @return 0 on success, error code on failure.
 */
int create_thread(pthread_t *thread_id, void *(*start_routine) (void *), void *arg);

/**
 * @brief Computes the number of samples per frame based on sample rate.
 *
 * @param sample_rate The sample rate to use for the computation.
 * @return Number of samples per frame.
 */
int compute_numPerFrm(int sample_rate);

// Cleans up all resources and prepares for program termination.
void perform_cleanup(void);

// Handles the SIGINT signal to allow the program to exit gracefully.
void handle_sigint(int sig);

// Transforms the program into a daemon process.
void daemonize(void);

// Sets up signal handling for the program.
void setup_signal_handling(void);

/**
 * @brief Checks if another instance of the program is already running.
 *
 * @return 1 if another instance is running, 0 otherwise.
 */
int is_already_running(void);

// Flag to indicate if the thread should stop.
extern volatile int g_stop_thread;

// Mutex associated with the stop thread flag.
extern pthread_mutex_t g_stop_thread_mutex;

#endif // UTILS_H

/**
 * @brief Initialize the configuration system by loading the provided file.
 *
 * @param config_file_path Path to the configuration file to load.
 * @return int 0 on success, -1 on error.
 */
int config_load_from_file(const char *config_file_path);

/**
 * @brief Cleanup and free resources associated with the configuration system.
 */
void config_cleanup(void);

// -----------------------------
// Configuration Retrieval Functions
// -----------------------------

/**
 * @brief Retrieve the 'audio' configuration object.
 *
 * @return cJSON* Pointer to the 'audio' cJSON object, or NULL if not found.
 */
cJSON *get_audio_config(void);

/**
 * @brief Retrieve the 'network' configuration object.
 *
 * @return cJSON* Pointer to the 'network' cJSON object, or NULL if not found.
 */
cJSON *get_network_config(void);

/**
 * @brief Check if AI (Audio Input) is enabled in the configuration.
 *
 * @return int 1 if enabled, 0 otherwise.
 */
int config_get_ai_enabled(void);

/**
 * @brief Check if AO (Audio Output) is enabled in the configuration.
 *
 * @return int 1 if enabled, 0 otherwise.
 */
int config_get_ao_enabled(void);

// -----------------------------
// Socket Path Retrieval Functions
// -----------------------------

/**
 * @brief Retrieve the AI (Audio Input) socket path from the configuration.
 *
 * @return char* The AI socket path. Caller is responsible for freeing the returned string using free().
 */
char* config_get_ai_socket(void);

/**
 * @brief Retrieve the AO (Audio Output) socket path from the configuration.
 *
 * @return char* The AO socket path. Caller is responsible for freeing the returned string using free().
 */
char* config_get_ao_socket(void);

/**
 * @brief Retrieve the control socket path from the configuration.
 *
 * @return char* The control socket path. Caller is responsible for freeing the returned string using free().
 */
char* config_get_ctrl_socket(void);

// -----------------------------
// Audio Attribute Retrieval Functions
// -----------------------------

/**
 * @brief Enum to differentiate between audio input and output.
 */
typedef enum {
    AUDIO_INPUT,
    AUDIO_OUTPUT
} AudioType;

/**
 * @brief Retrieve a specific attribute from the audio configuration.
 *
 * @param type The type of audio (input or output).
 * @param attribute_name The name of the attribute to retrieve.
 * @return cJSON* Pointer to the attribute's cJSON object, or NULL if not found.
 */
cJSON* get_audio_attribute(AudioType type, const char* attribute_name);

/**
 * @brief Retrieve the AO (Audio Output) frame size from the configuration.
 *
 * @return int The AO frame size, or DEFAULT_AO_MAX_FRAME_SIZE if not found in the configuration.
 */
int config_get_ao_frame_size(void);

/**
 * Checks if the provided samplerate is valid.
 * @param samplerate The samplerate value to be checked.
 * @return 1 if the samplerate is valid, 0 otherwise.
 */
int is_valid_samplerate(int samplerate);

/**
 * @brief Validates the loaded configuration JSON for correct structure and keys.
 *
 * @param root The root cJSON object of the loaded configuration.
 * @return int 1 if the configuration is valid, 0 otherwise.
 */
int validate_json(cJSON *root);

#endif // CONFIG_H
