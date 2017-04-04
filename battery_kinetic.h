// ANDRE RIKER - ARIKER AT DEI.UC.PT
// UNIVERSIDADE DE COIMBRA


// ------------ STRUCTS --------------
struct battery;

struct stats;

struct energy_states;

// ----------- FUNCTIONS -------------
void update_battery();

void update_time_stats();

void battery_start(clock_time_t perioc, unsigned, double, unsigned);

void kinetic_model();

long double get_battery_charge();

double get_max_charge();

double get_max_available_charge();
