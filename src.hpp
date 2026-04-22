// Implementation for Problem 059 - OJ 的继承与派生
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>

class BaseJudger {
public:
    BaseJudger(size_t time_limit, size_t memory_limit, const char* answer)
        : time_limit_(time_limit), memory_limit_(memory_limit), score_(0) {
        // allocate and copy answer
        if (answer) {
            size_t n = std::strlen(answer);
            answer_ = new char[n + 1];
            std::memcpy(answer_, answer, n + 1);
        } else {
            answer_ = new char[1];
            answer_[0] = '\0';
        }
    }

    virtual void Submit(size_t time, size_t memory, const char* output) = 0;

    size_t GetScore() const { return score_; }

    virtual ~BaseJudger() {
        // free allocated answer
        delete[] answer_;
    };

protected:
    char* answer_;
    const size_t time_limit_;
    const size_t memory_limit_;
    size_t score_;

    virtual bool CheckAnswer(const char* output) const {
        // the output must equal to the answer
        return std::strcmp(answer_, output) == 0;
    }
};


class ICPCJudger : public BaseJudger {
public:
    ICPCJudger(size_t time_limit, size_t memory_limit, const char* answer)
        : BaseJudger(time_limit, memory_limit, answer) {}

    void Submit(size_t time, size_t memory, const char* output) override {
        size_t cur = 0;
        if (time <= time_limit_ && memory <= memory_limit_ && CheckAnswer(output)) {
            cur = 100;
        }
        if (cur > score_) score_ = cur;
    }
};


class OIJudger : public BaseJudger {
public:
    OIJudger(size_t time_limit, size_t memory_limit, const char* answer)
        : BaseJudger(time_limit, memory_limit, answer), submitted_(false) {}

    void Submit(size_t time, size_t memory, const char* output) override {
        if (submitted_) {
            // multiple submits explode to zero
            score_ = 0;
            return;
        }
        submitted_ = true;
        if (time <= time_limit_ && memory <= memory_limit_ && CheckAnswer(output)) {
            score_ = 100;
        } else {
            score_ = 0;
        }
    }

protected:
    // Compare answer and output ignoring trailing spaces per line
    bool CheckAnswer(const char* output) const override {
        const char* a = answer_;
        const char* b = output ? output : "";
        while (true) {
            // find end of current line
            const char* a_end = std::strchr(a, '\n');
            const char* b_end = std::strchr(b, '\n');
            // If one has no more lines, both must end simultaneously
            if (!a_end || !b_end) {
                // Compare the last segment (possibly entire string) without trailing spaces
                const char* a_line_end = a + std::strlen(a);
                const char* b_line_end = b + std::strlen(b);
                // trim trailing spaces/tabs/CR
                while (a_line_end > a && (*(a_line_end - 1) == ' ' || *(a_line_end - 1) == '\t' || *(a_line_end - 1) == '\r')) {
                    --a_line_end;
                }
                while (b_line_end > b && (*(b_line_end - 1) == ' ' || *(b_line_end - 1) == '\t' || *(b_line_end - 1) == '\r')) {
                    --b_line_end;
                }
                size_t a_len = static_cast<size_t>(a_line_end - a);
                size_t b_len = static_cast<size_t>(b_line_end - b);
                if (a_len != b_len) return false;
                if (std::memcmp(a, b, a_len) != 0) return false;
                // Must both end here
                return (!a_end && !b_end);
            }

            // compare the current line ignoring trailing spaces
            const char* a_trim_end = a_end;
            const char* b_trim_end = b_end;
            while (a_trim_end > a && (*(a_trim_end - 1) == ' ' || *(a_trim_end - 1) == '\t' || *(a_trim_end - 1) == '\r')) {
                --a_trim_end;
            }
            while (b_trim_end > b && (*(b_trim_end - 1) == ' ' || *(b_trim_end - 1) == '\t' || *(b_trim_end - 1) == '\r')) {
                --b_trim_end;
            }
            size_t a_len = static_cast<size_t>(a_trim_end - a);
            size_t b_len = static_cast<size_t>(b_trim_end - b);
            if (a_len != b_len) return false;
            if (std::memcmp(a, b, a_len) != 0) return false;

            // advance to next line (skip newline)
            a = a_end + 1;
            b = b_end + 1;
        }
    }

private:
    bool submitted_;
};


class SpacialJudger : public BaseJudger {
public:
    SpacialJudger(size_t time_limit, size_t memory_limit,
                  size_t full_score_time, size_t full_score_memory,
                  const char* answer)
        : BaseJudger(time_limit, memory_limit, answer),
          full_score_time_(full_score_time), full_score_memory_(full_score_memory) {}

    void Submit(size_t time, size_t memory, const char* output) override {
        size_t cur = 0;
        if (time <= time_limit_ && memory <= memory_limit_ && CheckAnswer(output)) {
            int time_score = compute_metric_score(time, full_score_time_, time_limit_);
            int mem_score = compute_metric_score(memory, full_score_memory_, memory_limit_);
            long long combined = static_cast<long long>(time_score) * static_cast<long long>(mem_score);
            cur = static_cast<size_t>(combined / 100); // floor by integer division
        }
        if (cur > score_) score_ = cur;
    }

private:
    size_t full_score_time_;
    size_t full_score_memory_;

    static int compute_metric_score(size_t val, size_t full_score_val, size_t limit_val) {
        if (val <= full_score_val) return 100;
        if (val >= limit_val) return 0;
        // linear between (full_score_val, 100) and (limit_val, 0)
        size_t denom = (limit_val > full_score_val) ? (limit_val - full_score_val) : 1; // avoid zero
        size_t num = limit_val - val;
        // floor
        return static_cast<int>((100ULL * num) / denom);
    }
};

