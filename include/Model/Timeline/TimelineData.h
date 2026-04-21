#pragma once
#include <vector>

template <typename State, typename Operation, typename Record>
class TimelineData {
public:
    void setInitialState(const State& state) {
        initial_state = state;
    }
    State getInitialState() const {
        return initial_state;
    }
    void push(const State& state, const State& f_state, const Operation& op, const Record& record) {
        states.push_back(state);
        finish_states.push_back(f_state);
        operations.push_back(op);
        records.push_back(record);
    }

    void clear() {
        states.clear();
        finish_states.clear();
        operations.clear();
        records.clear();
    }

    bool empty() const {
        return states.empty();
    }

    std::size_t size() const {
        return states.size();
    }

    const State& getState(std::size_t index) const {
        return states[index];
    }

    const State& getFinishState(std::size_t index) const {
        return finish_states[index];
    }

    const Operation& getOperation(std::size_t index) const {
        //std::cout << index << " " << operations.size() << "\n";
        return operations[index];
    }

    const Record& getRecord(std::size_t index) const {
        return records[index];
    }

private:
    State initial_state;
    std::vector<State> states;
    std::vector<State> finish_states;
    std::vector<Operation> operations;
    std::vector<Record> records;
};