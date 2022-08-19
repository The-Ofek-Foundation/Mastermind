#include "mastermind.hpp"

#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <vector>

Mastermind::Mastermind(unsigned num_colors, unsigned num_pins)
    : _num_guesses(0u), _num_colors(num_colors), _num_pins(num_pins)
{
    NUM_POSSIBILITIES = std::pow(num_colors, num_pins);

    for (unsigned i = 0u; i < NUM_POSSIBILITIES; ++i)
    {
        _possibilities.insert(i);
    }
}

void Mastermind::add_response(unsigned guess, const Response& response) noexcept
{
    _num_guesses++;

    auto possibilities = _possibilities;

    for (const auto& possibility : possibilities)
    {
        if (get_response(guess, possibility) != response)
        {
            _possibilities.erase(possibility);
        }
    }
}

EntropyGuess Mastermind::get_highest_entropy() const noexcept
{
    EntropyGuess highest_entropy_guess = {0u, 0.0};

    for (unsigned guess = 0u; guess < NUM_POSSIBILITIES; ++guess)
    {
        double entropy = calculate_entropy(guess);

        if (entropy > highest_entropy_guess.entropy)
        {
            highest_entropy_guess = {guess, entropy};
        }
    }

    return highest_entropy_guess;
}

double Mastermind::calculate_entropy(unsigned guess) const noexcept
{
    std::unordered_map<unsigned, std::unordered_map<unsigned, unsigned>> counts;

    // get counts for each possibility
    for (const auto& possibility : _possibilities)
    {
        Response response = get_response(possibility, guess);
        ++counts[response.correct_positions][response.correct_colors];
    }

    // calculate entropy
    double entropy = 0.0;
    for (const auto& count : counts)
    {
        for (const auto& inner_count : count.second)
        {
            double p = static_cast<double>(inner_count.second) / _possibilities.size();
            entropy += p * std::log2(p);
        }
    }

    return -entropy;
}

Response Mastermind::get_response(unsigned guess, unsigned solution) const noexcept
{
    Response response;

    std::unordered_map<unsigned, int> out_of_place;

    // get black pins
    for (unsigned i = 0u; i < _num_pins; ++i, guess /= _num_colors, solution /= _num_colors)
    {
        if ((guess % _num_colors) == (solution % _num_colors))
        {
            ++response.correct_positions;
            continue;
        }

        // check incorrect positions
        int& count_guess = out_of_place[guess % _num_colors];
        if (count_guess < 0)
        {
            ++response.correct_colors;
        }

        ++count_guess;

        int& count_solution = out_of_place[solution % _num_colors];
        if (count_solution > 0)
        {
            ++response.correct_colors;
        }

        --count_solution;
    }

    return response;
}

unsigned Mastermind::vector_to_guess(const std::vector<unsigned>& v) const noexcept
{
    unsigned guess = 0u;
    unsigned p = 1u;

    for (const auto& color : v)
    {
        guess += color * p;
        p *= _num_colors;
    }

    return guess;
}

std::vector<unsigned> Mastermind::guess_to_vector(unsigned guess) const noexcept
{
    std::vector<unsigned> v;
    for (unsigned i = 0u; i < _num_pins; ++i, guess /= _num_colors)
    {
        v.emplace_back(guess % _num_colors);
    }

    return v;
}
