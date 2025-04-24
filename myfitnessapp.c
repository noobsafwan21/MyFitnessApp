#include <stdio.h>
#include <string.h>
 
 #define MAX_WORKOUTS 10
 #define MAX_GOAL_LENGTH 50
 
 struct FitnessCoach {
     char name[50];
     char sex;
     int age;
     char goal[MAX_GOAL_LENGTH];
     float startingWeight;
     float currentWeight;
     int goalTime;
     char exercises[MAX_WORKOUTS][50];
     int exerciseCount;
     float caloriesBurnt;
 };
 
 // Function prototypes
 void inputUserInfo(struct FitnessCoach *user);
 void showExerciseSuggestions(const struct FitnessCoach *user);
 void showDietPlan(const struct FitnessCoach *user);
 void showPersonalizedWorkoutPlan(const struct FitnessCoach *user);
 void logWorkout(struct FitnessCoach *user);
 void showProgress(const struct FitnessCoach *user);
 void trackProgress(struct FitnessCoach *user);
 void adjustIntensityMessage(int goalTime);
 float getCaloriesForExercise(const char *exercise);
 void toLowercase(char *str);
 void saveUserData(const struct FitnessCoach *user);
 int loadUserData(struct FitnessCoach *user);
 
 int main() {
     struct FitnessCoach user;
     int choice;
 
     if (loadUserData(&user)) {
         printf("\nWelcome back, %s!\n", user.name);
     } else {
         printf("\nNo existing data found. Let's get started!\n");
         inputUserInfo(&user);
     }
 
     while (1) {
         printf("\n--- Virtual Fitness Coach ---\n");
         printf("1. Show Exercise Suggestions\n");
         printf("2. Show Diet Plan\n");
         printf("3. Show Workout Plan\n");
         printf("4. Log Workout\n");
         printf("5. Show Progress\n");
         printf("6. Track Weight\n");
         printf("7. Exit\n");
         printf("Enter your choice: ");
         
         if (scanf("%d", &choice) != 1) {
             printf("Invalid input! Please enter a number.\n");
             while(getchar() != '\n');
             continue;
         }
         while(getchar() != '\n');
 
         switch (choice) {
             case 1: showExerciseSuggestions(&user); break;
             case 2: showDietPlan(&user); break;
             case 3: showPersonalizedWorkoutPlan(&user); break;
             case 4: logWorkout(&user); break;
             case 5: showProgress(&user); break;
             case 6: trackProgress(&user); break;
             case 7: 
                 saveUserData(&user);
                 printf("Data saved. Exiting program.\n"); 
                 return 0;
             default: printf("Invalid choice! Please try again.\n");
         }
     }
 }
 
 void saveUserData(const struct FitnessCoach *user) {
     FILE *file = fopen("fitness_data.dat", "wb");
     if (file == NULL) {
         printf("Error saving data!\n");
         return;
     }
     fwrite(user, sizeof(struct FitnessCoach), 1, file);
     fclose(file);
 }
 
 int loadUserData(struct FitnessCoach *user) {
     FILE *file = fopen("fitness_data.dat", "rb");
     if (file == NULL) return 0;
     
     int result = fread(user, sizeof(struct FitnessCoach), 1, file);
     fclose(file);
     return result == 1;
 }
 
 void inputUserInfo(struct FitnessCoach *user) {
     const char* goals[] = {
         "weight loss",
         "weight gain",
         "muscle build",
         "endurance"
     };
     const int goalCount = 4;
     int choice;
 
     printf("Enter your name: ");
     fgets(user->name, sizeof(user->name), stdin);
     user->name[strcspn(user->name, "\n")] = '\0';
 
     int valid;
     do {
         valid = 1;
         printf("Enter your age: ");
         if (scanf("%d", &user->age) != 1) {
             printf("Invalid input! Please enter a number.\n");
             valid = 0;
         }
         while(getchar() != '\n');
     } while (!valid || user->age <= 0);
 
     do {
         printf("Enter your sex (M/F): ");
         user->sex = getchar();
         while(getchar() != '\n');
         
         if(user->sex >= 'a' && user->sex <= 'z') {
             user->sex -= 32;
         }
     } while(user->sex != 'M' && user->sex != 'F');
 
     printf("\nChoose your fitness goal:\n");
     for(int i = 0; i < goalCount; i++) {
         printf("%d. %s\n", i+1, goals[i]);
     }
     
     do {
         printf("Enter choice (1-4): ");
         if (scanf("%d", &choice) != 1) {
             printf("Invalid input! Please enter a number.\n");
             choice = 0;
         }
         while(getchar() != '\n');
     } while(choice < 1 || choice > 4);
 
     strncpy(user->goal, goals[choice-1], MAX_GOAL_LENGTH);
     toLowercase(user->goal);
 
     do {
         printf("Enter current weight (kg): ");
         if (scanf("%f", &user->currentWeight) != 1) {
             printf("Invalid input! Please enter a number.\n");
             user->currentWeight = 0;
         }
         while(getchar() != '\n');
     } while(user->currentWeight <= 0);
     user->startingWeight = user->currentWeight;
 
     do {
         printf("Goal timeline (weeks): ");
         if (scanf("%d", &user->goalTime) != 1) {
             printf("Invalid input! Please enter a number.\n");
             user->goalTime = 0;
         }
         while(getchar() != '\n');
     } while(user->goalTime <= 0);
 
     user->exerciseCount = 0;
     user->caloriesBurnt = 0;
 }
 
 void toLowercase(char *str) {
     for(int i = 0; str[i]; i++) {
         if(str[i] >= 'A' && str[i] <= 'Z') {
             str[i] += 32;
         }
     }
 }
 
 void showExerciseSuggestions(const struct FitnessCoach *user) {
     printf("\n--- Suggested Exercises for %s ---\n", user->name);
     
     if (strstr(user->goal, "loss")) {
         printf("- High Intensity Interval Training (HIIT)\n- Cycling\n- Swimming\n- Jump Rope\n");
     }
     else if (strstr(user->goal, "gain")) {
         printf("- Compound Lifts (Squats/Deadlifts)\n- Resistance Training\n- Calorie-Dense Meal Prep\n");
     }
     else if (strstr(user->goal, "build")) {
         printf("- Weight Lifting\n- Pyramid Sets\n- Drop Sets\n- Progressive Overload\n");
     }
     else if (strstr(user->goal, "endurance")) {
         printf("- Long Distance Running\n- Cycling\n- Rowing\n- Circuit Training\n");
     }
 }
 
 void showDietPlan(const struct FitnessCoach *user) {
     printf("\n--- Nutrition Plan for %s ---\n", user->name);
     adjustIntensityMessage(user->goalTime);
 
     if (strstr(user->goal, "loss")) {
         printf("\nCalorie Deficit: 500 kcal/day\n");
         printf("Macros: 40%% protein, 30%% fat, 30%% carbs\n");
         printf("\nSample Daily Meals:\n");
         printf("Breakfast: Egg white omelette with veggies\n");
         printf("Lunch: Grilled chicken salad with olive oil\n");
         printf("Dinner: Baked salmon with asparagus\n");
         printf("Snack: Greek yogurt with berries\n");
     }
     else if (strstr(user->goal, "gain")) {
         printf("\nCalorie Surplus: 500 kcal/day\n");
         printf("Macros: 50%% carbs, 30%% protein, 20%% fats\n");
         printf("\nSample Daily Meals:\n");
         printf("Breakfast: Oatmeal with peanut butter + banana\n");
         printf("Lunch: Beef stir-fry with rice\n");
         printf("Dinner: Pasta with meat sauce\n");
         printf("Snack: Protein shake with milk\n");
     }
     else if (strstr(user->goal, "build")) {
         printf("\nMaintenance Calories\n");
         printf("Macros: 40%% protein, 40%% carbs, 20%% fats\n");
         printf("\nSample Daily Meals:\n");
         printf("Breakfast: 6 egg whites + whole grain toast\n");
         printf("Lunch: Grilled chicken with sweet potato\n");
         printf("Dinner: Steak with quinoa and broccoli\n");
         printf("Snack: Cottage cheese with pineapple\n");
     }
     else if (strstr(user->goal, "endurance")) {
         printf("\nBalanced Calories\n");
         printf("Macros: 50%% carbs, 30%% protein, 20%% fats\n");
         printf("\nSample Daily Meals:\n");
         printf("Breakfast: Whole grain pancakes with honey\n");
         printf("Lunch: Turkey sandwich on whole wheat\n");
         printf("Dinner: Salmon with brown rice\n");
         printf("Snack: Energy bars\n");
     }
 }
 
 void showPersonalizedWorkoutPlan(const struct FitnessCoach *user) {
     printf("\n--- Custom Workout Plan ---\n");
     adjustIntensityMessage(user->goalTime);
     
     if(strstr(user->goal, "loss")) {
         printf("\nWeight Loss Program (4 days/week)\n");
         printf("Day 1: HIIT Circuit (3 rounds)\n");
         printf("1. Burpees         3 x 45 sec\n");
         printf("2. Jump Squats     3 x 45 sec\n");
         printf("3. Mountain Climbers 3 x 45 sec\n\n");
         
         printf("Day 2: Cardio & Core\n");
         printf("1. Treadmill Intervals 20 min\n");
         printf("2. Plank          3 x 60 sec\n");
         printf("3. Russian Twists 3 x 20 reps\n\n");
     }
     else if(strstr(user->goal, "gain")) {
         printf("\nWeight Gain Program (5 days/week)\n");
         printf("Day 1: Chest & Triceps\n");
         printf("1. Bench Press     4 x 8-10\n");
         printf("2. Incline Press   3 x 10-12\n");
         printf("3. Tricep Dips     3 x 12-15\n\n");
         
         printf("Day 2: Back & Biceps\n");
         printf("1. Deadlifts       4 x 6-8\n");
         printf("2. Lat Pulldowns   3 x 10-12\n");
         printf("3. Barbell Curls   3 x 12-15\n\n");
     }
     else if(strstr(user->goal, "build")) {
         printf("\nMuscle Building Program (6 days/week)\n");
         printf("Day 1: Chest & Triceps\n");
         printf("1. Flat Bench      5 x 5\n");
         printf("2. Incline Bench   4 x 8\n");
         printf("3. Cable Crossovers 3 x 12-15\n\n");
         
         printf("Day 2: Back & Biceps\n");
         printf("1. Pull-ups        4 x 8-10\n");
         printf("2. Bent-over Rows  4 x 8\n");
         printf("3. Barbell Curls   3 x 10-12\n\n");
     }
     else if(strstr(user->goal, "endurance")) {
         printf("\nEndurance Program (5 days/week)\n");
         printf("Day 1: Cardio Endurance\n");
         printf("1. Long Run        60 min\n");
         printf("2. Bodyweight Circuit 3 rounds\n\n");
         
         printf("Day 2: Swimming\n");
         printf("1. Freestyle Intervals 30 min\n");
         printf("2. Treading Water 10 min\n\n");
     }
     
     printf("General Guidelines:\n");
     printf("- Warm up 10 min before each session\n");
     printf("- Rest 60-90 sec between sets\n");
     printf("- Increase intensity weekly\n");
 }
 
 void logWorkout(struct FitnessCoach *user) {
     if (user->exerciseCount >= MAX_WORKOUTS) {
         printf("Workout log full! Maximum %d workouts.\n", MAX_WORKOUTS);
         return;
     }
 
     char exercise[50];
     printf("Enter exercise name: ");
     fgets(exercise, sizeof(exercise), stdin);
     exercise[strcspn(exercise, "\n")] = '\0';
 
     if (strlen(exercise) == 0) {
         printf("Exercise name cannot be empty!\n");
         return;
     }
 
     strcpy(user->exercises[user->exerciseCount], exercise);
     user->caloriesBurnt += getCaloriesForExercise(exercise);
     user->exerciseCount++;
     printf("Added: %s\n", exercise);
 }
 
 void showProgress(const struct FitnessCoach *user) {
     printf("\n--- Progress Report ---\n");
     printf("Weight Tracking:\n");
     printf("Starting: %.1fkg | Current: %.1fkg | Change: %.1fkg\n", 
           user->startingWeight, user->currentWeight,
           user->currentWeight - user->startingWeight);
     
     printf("\nCalories Burnt: %.0f kcal\n", user->caloriesBurnt);
     printf("Logged Exercises (%d):\n", user->exerciseCount);
     for(int i = 0; i < user->exerciseCount; i++) {
         printf("%d. %s\n", i+1, user->exercises[i]);
     }
 }
 
 void trackProgress(struct FitnessCoach *user) {
     do {
         printf("Enter new weight (kg): ");
         if (scanf("%f", &user->currentWeight) != 1) {
             printf("Invalid input! Please enter a number.\n");
             user->currentWeight = 0;
         }
         while(getchar() != '\n');
     } while(user->currentWeight <= 0);
     printf("Weight updated!\n");
 }
 
 void adjustIntensityMessage(int goalTime) {
     if (goalTime <= 4) 
         printf("Intensity: High (Short-term goal)\n");
     else if (goalTime <= 8) 
         printf("Intensity: Moderate (Medium-term)\n");
     else 
         printf("Intensity: Progressive (Long-term)\n");
 }
 
 float getCaloriesForExercise(const char *exercise) {
     struct ExerciseCalories {
         const char *name;
         float calories;
     };
     
     const struct ExerciseCalories database[] = {
         {"running", 600}, {"cycling", 500}, {"swimming", 700},
         {"weight lifting", 300}, {"yoga", 250}, {"boxing", 800},
         {"walking", 300}, {"hiit", 900}, {"rowing", 600}
     };
 
     char lowerExercise[50];
     strcpy(lowerExercise, exercise);
     toLowercase(lowerExercise);
 
     for(int i = 0; i < sizeof(database)/sizeof(database[0]); i++) {
         if(strstr(lowerExercise, database[i].name))
             return database[i].calories;
     }
     return 200; 
 }