#include <sparki.h>

int distance = 0; // distance is the global variable to record the distance to the right black line

bool find_black_line() { 
    // Think about which condition satisfied to return true, otherwise return false
    int centerVal = sparki.lineCenter();   // Read center line sensor
    int rightVal  = sparki.lineRight();    // Read right line sensor

    if (centerVal < 400 || rightVal < 400) // Detect black line if below threshold
        return true;
    return false;
}

int find_distance() {
    // Use Accumulator algorithm to find the distance to the right black line
    long sum = 0;                          // Initialize sum for averaging
    int count = 0;                         // Counter for valid samples
    int reading;

    for (int i = 0; i < 10; i++) {         // Take multiple ultrasonic readings
        reading = sparki.ping();           // Get distance in cm
        if (reading > 0) {                 // Ignore invalid zero readings
            sum += reading;
            count++;
        }
        delay(30);                         // Wait between readings
    }

    if (count == 0) return 3;              // Default if no valid readings
    return (int)(sum / count);             // Return average distance
}

void check_intruder() {
    int d = sparki.ping();                 // Measure front distance
    if (d > 0 && d < 20) {                 // If obstacle is within 20 cm
        sparki.moveStop();                 // Stop movement
        sparki.RGB(255, 0, 0);             // Turn LED red
        sparki.moveBackward(5);            // Move back 5 cm
        sparki.RGB(0, 0, 0);               // Turn off LED
    }
} // You may need to implement this function at a later stage

void forward(int dist) { 
    sparki.moveForward(dist);              //Move forward by given distance
    delay(dist * 60);                      //Wait for completion (approx.)
    sparki.moveStop();                     // Stop motors
} // You may need to implement this function at a later stage

void setup() {
    distance = find_distance();
    sparki.moveBackward(distance);
}

void loop() { // Firstly, think about Sparki's moving pattern, similar to drawing a square, but more complex.
    int step = 3; // Local variable that stores the step size (cm) for moving up or down

    sparki.moveForward(distance); // You may need to call your own forward function at a later stage.
    sparki.moveRight(90);
    sparki.moveForward(step);
    if (find_black_line()) {
        sparki.moveBackward(step);
        /*
        Fill out the next steps
        */
        sparki.RGB(0, 255, 0);             // Flash green LED on black line detection
        delay(200);                        // Short delay
        sparki.RGB(0, 0, 0);               // Turn off LED
        sparki.moveLeft(90);               // Turn left to avoid black line
        sparki.moveForward(step);          // Move forward slightly
        check_intruder();                  // Check for obstacle ahead
    }
    sparki.moveRight(90);

    /*
    Fill out the next steps
    */
    sparki.moveForward(distance);          // Continue moving in square pattern
}
}
