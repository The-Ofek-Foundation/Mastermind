#include <unordered_set>
#include <vector>

struct Response
{
    unsigned correct_positions = 0u; // black pegs
    unsigned correct_colors = 0u;    // white pegs

    bool operator==(const Response& other) const noexcept
    {
        return correct_positions == other.correct_positions && correct_colors == other.correct_colors;
    }

    bool operator!=(const Response& other) const noexcept
    {
        return !(*this == other);
    }
};

struct EntropyGuess
{
    unsigned guess;
    double entropy;
};

class Mastermind
{
public:
    Mastermind(unsigned num_colors, unsigned num_pins);

    void add_response(unsigned guess, const Response& response) noexcept;

    EntropyGuess get_highest_entropy() const noexcept;

    double calculate_entropy(unsigned guess) const noexcept;

    Response get_response(unsigned guess, unsigned solution) const noexcept;

    unsigned vector_to_guess(const std::vector<unsigned>& v) const noexcept;

    std::vector<unsigned> guess_to_vector(unsigned guess) const noexcept;

    std::unordered_set<unsigned> _possibilities;

private:
    unsigned _num_guesses;
    unsigned _num_colors, _num_pins;
    unsigned NUM_POSSIBILITIES;
};
