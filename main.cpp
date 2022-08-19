#include "src/mastermind.hpp"

#include <cstdio>
#include <stdio.h>

void print_guess(const Mastermind& m, unsigned guess)
{
    printf("(%u) ", guess);
    for (unsigned color : m.guess_to_vector(guess))
    {
        printf("%u ", color);
    }
    printf("\n");
}

void print_entropy_guess(const Mastermind& m, const EntropyGuess& guess)
{

    printf("Guess: ");
    print_guess(m, guess.guess);

    printf("Entropy: %f\n", guess.entropy);
}

Response get_response_input()
{
    Response response;
    printf("Enter number black: ");
    scanf("%u", &response.correct_positions);
    printf("Enter number white: ");
    scanf("%u", &response.correct_colors);

    return response;
}

int main()
{
    Mastermind m(6u, 4u);

    while (m._possibilities.size() > 2)
    {
        printf("Num possibilities: %lu\n", m._possibilities.size());

        EntropyGuess guess = m.get_highest_entropy();
        print_entropy_guess(m, guess);

        printf("\n");

        m.add_response(guess.guess, get_response_input());
    }

    printf("Num possibilities: %lu\n", m._possibilities.size());

    for (const auto& possibility : m._possibilities)
    {
        print_guess(m, possibility);
    }
}
