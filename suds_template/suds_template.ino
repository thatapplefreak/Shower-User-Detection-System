// Code Running on SUDS arduinos in CSH Bathrooms

const String BATHROOM_ID = "CHANGE THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";

// Detection Pins
const int CLOSE_STALL = 12;
const int FAR_STALL = 13;

// LEDs outside the bathroom
const int CLOSE_LED = 5;
const int FAR_LED = 6;

//EVENT Identifier IDs
const int EVENT_STALL_ENTER = 11111995;
const int EVENT_STALL_EXIT = 11111996;

//IS SOMEBODY TAKING A SHOWER?
boolean close_occupied = false;
boolean far_occupied = false;

void setup() {
  // These wires go to the stalls
  pinMode(CLOSE_STALL, INPUT);
  pinMode(FAR_STALL, INPUT);
  // These wires go to the box ouside the bathroom
  pinMode(CLOSE_LED, OUTPUT);
  pinMode(FAR_LED, OUTPUT);

  // Open the serial port
  Serial.begin(9600);
}

void loop() {

  // Don't pick up random noise or a tap on the switch
  delay(3000);
  
  // Read the stall switches
  // True if the switch in the stall is closed
  boolean close_status = digitalRead(CLOSE_STALL) == HIGH;
  boolean far_status = digitalRead(FAR_STALL) == HIGH;

  // Find discrepincies
  // will be true if something has changed
  boolean close_swap = close_status ^ close_occupied;
  boolean far_swap = far_status ^ far_occupied;

  // Log the difference and swap the occupied value
  if (close_swap) {
    log_event(CLOSE_STALL, close_occupied ? EVENT_STALL_EXIT : EVENT_STALL_ENTER);
    close_occupied = !close_occupied;
  }
  if (far_swap) {
    log_event(FAR_STALL, far_occupied ? EVENT_STALL_EXIT : EVENT_STALL_ENTER);
    far_occupied = !far_occupied;
  }

  digitalWrite(CLOSE_LED, close_occupied ? HIGH : LOW);
  digitalWrite(FAR_LED, far_occupied ? HIGH : LOW);
}

void log_event(int stall, int event) {
  String message = "EVENT: User has ";
  switch (event) {
    case EVENT_STALL_ENTER:
      message += "entered";
      break;
    case EVENT_STALL_EXIT:
      message += "exit";
      break;
  }
  message += " the ";
  switch (stall) {
    case CLOSE_STALL:
      message += "close";
      break;
    case FAR_STALL:
      message += "far";
      break;
  }
  message += " stall";
  Serial.println(message);
}
