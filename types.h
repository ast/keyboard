// Possible key states
typedef enum {
  KEY_IS_UP,
  KEY_IS_DOWN,
  KEY_IS_GOING_UP,    // We increment the timer in this state
  KEY_IS_GOING_DOWN,  // We increment the timer in this state
} state_t;

// Possible events
typedef enum {
  KEY_PRESSED,    // Key is pressed
  KEY_DOWN,       // Key reached bottom
  KEY_RELEASED,   // Key was released
  KEY_UP,         // Key reached top
} event_t;

typedef struct {
  char midi_note:8;
  state_t       state:4; // Bit fields
  unsigned int  t    :7; // Lines up nicely to 16bits, t overflows at 4096
} key_t;

typedef struct {
  char top;
  char bottom;
} bank_t;

