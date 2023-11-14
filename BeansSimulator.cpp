<<<<<<< HEAD
#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>

// Global variables
int blackCount = 40, whiteCount = 60;
int sumWhite = 0, sumMixed = 0, sumBlack = 0;

bool enableMultithreading()
{
   char response;
   while ( true ) {
      std::cout << "Multithreading makes the calculations faster." << std::endl;
      std::cout << "Enable multithreading (Y/N): ";
      std::cin >> response;

      if ( response == 'Y' || response == 'y' ) {
         return true;
      }
      else if ( response == 'N' || response == 'n' ) {
         return false;
      }
      else {
         std::cout << "Invalid input. Please enter 'Y' or 'N'." << std::endl;
      }
   }
}

void printMainMenu()
{
   std::cout << "Welcome to the Bean Sorting Simulator!" << std::endl << std::endl;
   std::cout << "In this simulation, we have two bags, each containing 40 black beans and 60 white beans." << std::endl;
   std::cout << "We randomly pick one bean from each bag and categorize them into three columns: " << std::endl;
   std::cout << "1. White beans" << std::endl;
   std::cout << "2. Mixed beans" << std::endl;
   std::cout << "3. Black beans" << std::endl << std::endl;
   std::cout << "We'll calculate the percentages of these beans based on the number of tries you enter." << std::endl << std::endl;
   std::cout << "Expected distribution after a large number of trials:" << std::endl;
   std::cout << "White beans: 36%" << std::endl;
   std::cout << "Mixed beans: 48%" << std::endl;
   std::cout << "Black beans: 16%" << std::endl << std::endl;
   std::cout << "This program allows you to input the number of tries to estimate the percentages." << std::endl << std::endl;

   std::cout << "Developed by: " << std::endl;
   std::cout << std::endl;
   std::cout << "   GGGGG    AAAAAA  MMM   MMM   AAAAAA TTTTTTT  OOOOO  RRRRRR   OOOOO " << std::endl;
   std::cout << "  GG       AA   AA  MM M M MM  AA   AA   TTT   OO   OO RR   RR OO   OO " << std::endl;
   std::cout << " GG        AA   AA  MM  M  MM  AA   AA   TTT   OO   OO RRRRRR  OO   OO " << std::endl;
   std::cout << " GG   GGG  AAAAAAA  MM     MM  AAAAAAA   TTT   OO   OO RRRRRR  OO   OO " << std::endl;
   std::cout << "  GG    GG AA   AA  MM     MM  AA   AA   TTT   OO   OO RR  RR  OO   OO " << std::endl;
   std::cout << "   GGGGGG  AA   AA  MM     MM  AA   AA   TTT    OOOOO  RR   RR  OOOOO " << std::endl;
   std::cout << std::endl;
}

unsigned long long getValidNumberOfTries()
{
   unsigned long long numOfTries;
   while ( true ) {
      std::cout << "Enter the number of tries: ";
      if ( !( std::cin >> numOfTries ) ) {
         // If the input is not an integer, clear the input buffer
         std::cin.clear();
         std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
         std::cout << "Invalid input. Please enter a valid integer.\n";
      }
      else if ( numOfTries <= 0 )
         // If the input is 0 or negative, prompt again
         std::cout << "Number of tries should be a positive integer greater than zero.\n";
      else
         // Valid
         break;
   }
   std::cout << std::endl;

   return numOfTries;
}

void sortBeans()
{
   // Seed the random number generator
   std::srand( static_cast<unsigned int>( std::time( nullptr ) ) );

   std::vector<bool> vreca1;
   std::vector<bool> vreca2;

   for ( int i = 0; i < whiteCount; ++i ) {
      vreca1.push_back( true );
      vreca2.push_back( true );
   }

   for ( int i = 0; i < blackCount; ++i ) {
      vreca1.push_back( false );
      vreca2.push_back( false );
   }

   while ( !vreca1.empty() && !vreca2.empty() ) {
      // Generate random indices
      int index1 = std::rand() % vreca1.size();
      int index2 = std::rand() % vreca2.size();

      // Access elements at random indices
      bool val1 = vreca1[index1];
      bool val2 = vreca2[index2];

      // Both picked white
      if ( val1 && val2 )
         sumWhite++;
      // Both picked black
      else if ( !val1 && !val2 )
         sumBlack++;
      // Both picked mixed
      else
         sumMixed++;

      vreca1.erase( vreca1.begin() + index1 );
      vreca2.erase( vreca2.begin() + index2 );
   }
}

void printStats( unsigned long long numOfTries, std::chrono::steady_clock::time_point& startTime )
{
   // Calculate percentages using floating-point arithmetic
   double percentageWhite = static_cast<double>( sumWhite ) / numOfTries;
   double percentageMixed = static_cast<double>( sumMixed ) / numOfTries;
   double percentageBlack = static_cast<double>( sumBlack ) / numOfTries;

   auto endTime = std::chrono::steady_clock::now();
   std::chrono::duration<double> elapsedTime = endTime - startTime;

   std::cout << "After " << numOfTries << " tries, the stats are: " << std::endl;
   std::cout << "Percentage of white beans: " << percentageWhite << "%" << std::endl;
   std::cout << "Percentage of mixed beans: " << percentageMixed << "%" << std::endl;
   std::cout << "Percentage of black beans: " << percentageBlack << "%" << std::endl;
   std::cout << std::endl;
   std::cout << "Time taken: " << elapsedTime.count() << " seconds." << std::endl;
}

void sortBeansMultiThreading( unsigned long long numOfTries, std::chrono::steady_clock::time_point& startTime )
{
   for ( unsigned long long i = 0; i < numOfTries; i++ ) {
      sortBeans();
   }
}

int main()
{
   bool bTryAgain = true;
   bool bUseMultithreading = enableMultithreading();
   if ( bUseMultithreading )
      std::cout << "Multithreading enabled." << std::endl;
   else
      std::cout << "Multithreading disabled." << std::endl;
   printMainMenu();

   while ( bTryAgain ) {
      unsigned long long numOfTries = getValidNumberOfTries();
      auto startTime = std::chrono::steady_clock::now();

      if ( bUseMultithreading ) {
         std::vector<std::thread> threads;
         // Determine the number of threads based on the available hardware concurrency
         unsigned int numThreads = std::thread::hardware_concurrency();
         if ( numThreads == 0 )
            numThreads = 1;
         for ( unsigned int i = 0; i < numThreads; ++i ) {
            threads.emplace_back( sortBeansMultiThreading, numOfTries / numThreads, std::ref( startTime ) );
         }
         for ( auto& thread : threads ) {
            thread.join();
         }
      }
      else {
         for ( unsigned long long i = 0; i < numOfTries; i++ ) {
            sortBeans();
         }
      }

      printStats( numOfTries, startTime );

      char tryAgain;
      std::cout << "Would you like to try again? (Y/N): ";
      std::cin >> tryAgain;
      if ( tryAgain == 'N' || tryAgain == 'n' )
         bTryAgain = false;
   }

   return 0;
=======
#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>

// Global variables
int blackCount = 40, whiteCount = 60;
int sumWhite = 0, sumMixed = 0, sumBlack = 0;

bool enableMultithreading()
{
   char response;
   while ( true ) {
      std::cout << "Multithreading makes the calculations faster." << std::endl;
      std::cout << "Enable multithreading (Y/N): ";
      std::cin >> response;

      if ( response == 'Y' || response == 'y' ) {
         return true;
      }
      else if ( response == 'N' || response == 'n' ) {
         return false;
      }
      else {
         std::cout << "Invalid input. Please enter 'Y' or 'N'." << std::endl;
      }
   }
}

void printMainMenu()
{
   std::cout << "Welcome to the Bean Sorting Simulator!" << std::endl << std::endl;
   std::cout << "In this simulation, we have two bags, each containing 40 black beans and 60 white beans." << std::endl;
   std::cout << "We randomly pick one bean from each bag and categorize them into three columns: " << std::endl;
   std::cout << "1. White beans" << std::endl;
   std::cout << "2. Mixed beans" << std::endl;
   std::cout << "3. Black beans" << std::endl << std::endl;
   std::cout << "We'll calculate the percentages of these beans based on the number of tries you enter." << std::endl << std::endl;
   std::cout << "Expected distribution after a large number of trials:" << std::endl;
   std::cout << "White beans: 36%" << std::endl;
   std::cout << "Mixed beans: 48%" << std::endl;
   std::cout << "Black beans: 16%" << std::endl << std::endl;
   std::cout << "This program allows you to input the number of tries to estimate the percentages." << std::endl << std::endl;

   std::cout << "Developed by: " << std::endl;
   std::cout << std::endl;
   std::cout << "   GGGGG    AAAAAA  MMM   MMM   AAAAAA TTTTTTT  OOOOO  RRRRRR   OOOOO " << std::endl;
   std::cout << "  GG       AA   AA  MM M M MM  AA   AA   TTT   OO   OO RR   RR OO   OO " << std::endl;
   std::cout << " GG        AA   AA  MM  M  MM  AA   AA   TTT   OO   OO RRRRRR  OO   OO " << std::endl;
   std::cout << " GG   GGG  AAAAAAA  MM     MM  AAAAAAA   TTT   OO   OO RRRRRR  OO   OO " << std::endl;
   std::cout << "  GG    GG AA   AA  MM     MM  AA   AA   TTT   OO   OO RR  RR  OO   OO " << std::endl;
   std::cout << "   GGGGGG  AA   AA  MM     MM  AA   AA   TTT    OOOOO  RR   RR  OOOOO " << std::endl;
   std::cout << std::endl;
}

unsigned long long getValidNumberOfTries()
{
   unsigned long long numOfTries;
   while ( true ) {
      std::cout << "Enter the number of tries: ";
      if ( !( std::cin >> numOfTries ) ) {
         // If the input is not an integer, clear the input buffer
         std::cin.clear();
         std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
         std::cout << "Invalid input. Please enter a valid integer.\n";
      }
      else if ( numOfTries <= 0 )
         // If the input is 0 or negative, prompt again
         std::cout << "Number of tries should be a positive integer greater than zero.\n";
      else
         // Valid
         break;
   }
   std::cout << std::endl;

   return numOfTries;
}

void sortBeans()
{
   // Seed the random number generator
   std::srand( static_cast<unsigned int>( std::time( nullptr ) ) );

   std::vector<bool> vreca1;
   std::vector<bool> vreca2;

   for ( int i = 0; i < whiteCount; ++i ) {
      vreca1.push_back( true );
      vreca2.push_back( true );
   }

   for ( int i = 0; i < blackCount; ++i ) {
      vreca1.push_back( false );
      vreca2.push_back( false );
   }

   while ( !vreca1.empty() && !vreca2.empty() ) {
      // Generate random indices
      int index1 = std::rand() % vreca1.size();
      int index2 = std::rand() % vreca2.size();

      // Access elements at random indices
      bool val1 = vreca1[index1];
      bool val2 = vreca2[index2];

      // Both picked white
      if ( val1 && val2 )
         sumWhite++;
      // Both picked black
      else if ( !val1 && !val2 )
         sumBlack++;
      // Both picked mixed
      else
         sumMixed++;

      vreca1.erase( vreca1.begin() + index1 );
      vreca2.erase( vreca2.begin() + index2 );
   }
}

void printStats( unsigned long long numOfTries, std::chrono::steady_clock::time_point& startTime )
{
   // Calculate percentages using floating-point arithmetic
   double percentageWhite = static_cast<double>( sumWhite ) / numOfTries;
   double percentageMixed = static_cast<double>( sumMixed ) / numOfTries;
   double percentageBlack = static_cast<double>( sumBlack ) / numOfTries;

   auto endTime = std::chrono::steady_clock::now();
   std::chrono::duration<double> elapsedTime = endTime - startTime;

   std::cout << "After " << numOfTries << " tries, the stats are: " << std::endl;
   std::cout << "Percentage of white beans: " << percentageWhite << "%" << std::endl;
   std::cout << "Percentage of mixed beans: " << percentageMixed << "%" << std::endl;
   std::cout << "Percentage of black beans: " << percentageBlack << "%" << std::endl;
   std::cout << std::endl;
   std::cout << "Time taken: " << elapsedTime.count() << " seconds." << std::endl;
}

void sortBeansMultiThreading( unsigned long long numOfTries, std::chrono::steady_clock::time_point& startTime )
{
   for ( unsigned long long i = 0; i < numOfTries; i++ ) {
      sortBeans();
   }
}

int main()
{
   bool bTryAgain = true;
   bool bUseMultithreading = enableMultithreading();
   if ( bUseMultithreading )
      std::cout << "Multithreading enabled." << std::endl;
   else
      std::cout << "Multithreading disabled." << std::endl;
   printMainMenu();

   while ( bTryAgain ) {
      unsigned long long numOfTries = getValidNumberOfTries();
      auto startTime = std::chrono::steady_clock::now();

      if ( bUseMultithreading ) {
         std::vector<std::thread> threads;
         // Determine the number of threads based on the available hardware concurrency
         unsigned int numThreads = std::thread::hardware_concurrency();
         if ( numThreads == 0 )
            numThreads = 1;
         for ( unsigned int i = 0; i < numThreads; ++i ) {
            threads.emplace_back( sortBeansMultiThreading, numOfTries / numThreads, std::ref( startTime ) );
         }
         for ( auto& thread : threads ) {
            thread.join();
         }
      }
      else {
         for ( unsigned long long i = 0; i < numOfTries; i++ ) {
            sortBeans();
         }
      }

      printStats( numOfTries, startTime );

      char tryAgain;
      std::cout << "Would you like to try again? (Y/N): ";
      std::cin >> tryAgain;

      if ( tryAgain == 'N' || tryAgain == 'n' )
         bTryAgain = false;
   }

   return 0;
>>>>>>> 9c4d09372b69f46792679cb5fce43b97f44f8100
}