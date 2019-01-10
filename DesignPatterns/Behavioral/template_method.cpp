#include <iostream>


/**
 * The `Template Method` design pattern deffers some steps of an algorithm to the client code by
 * defining the skeleton of an algorithm but leaving out varying details for the client to decide.
 * Much like with the `Strategy` design pattern if the decision is made statically it's probably
 * better to use policy-based or traits-based design using templates.
 */

class Game {
public:
    explicit Game(int number_of_players) noexcept
        : number_of_players_{number_of_players}
    {
    }

    void run()
    {
        start();
        while (!have_winner()) {
            take_turn();
            std::cerr << "Player " << get_winner() << " wins!\n";
        }
    }

protected:
    virtual void start() = 0;
    virtual bool have_winner() = 0;
    virtual void take_turn() = 0;
    virtual int get_winner() = 0;

    int current_player_{0};
    int number_of_players_;
};

class Chess : public Game {
public:
    Chess()
        : Game{2} { };

protected:
    void start() override
    {
        std::cerr << "Starting a game of chess with " << number_of_players_ << " players\n";
    }

    bool have_winner() override
    {
        return turns_ == max_turns_;
    }

    void take_turn() override
    {
        std::cerr << "Turn " << turns_ << " taken by player " << current_player_ << "\n";
        ++turns_;
        current_player_ = (current_player_ + 1) % number_of_players_;
    }

    int get_winner() override { return current_player_; }

private:
    int turns_{0};
    int max_turns_{10};
};


int main()
{
    Chess chess;
    chess.run();
}
