#pragma once
#include <array>


class model {
public:
	model() {
		for (auto i = 0; i < No_of_chars; i++) {
			char_to_index_[i] = i + 1;
			index_to_char_[i + 1] = i;
		}
	}

	int char_to_index(int ch) const {
		return char_to_index_[ch];
	}

	int index_to_char(int symbol) const {
		return index_to_char_[symbol];
	}

	int cum_freq(int symbol) const {
		return cum_freq_[symbol];
	}

	int eof_symbol() const {
		return EOF_symbol;
	}

	virtual void update_model(int symbol) = 0;

	virtual ~model() = default;

protected:
	const static int No_of_chars = 256;
	const static int EOF_symbol = No_of_chars + 1;

	const static int No_of_symbols = No_of_chars + 1;
	const static int Max_frequency = 16383;

	std::array<int, No_of_chars> char_to_index_{};
	std::array<unsigned char, No_of_symbols + 1> index_to_char_{};

	std::array<int, No_of_symbols + 1> cum_freq_{};
	
};


class adaptive_model : public model {
public:
	adaptive_model() : model() {
		for (auto i = 0; i <= No_of_symbols; i++) {
			freq_[i] = 1;
			cum_freq_[i] = No_of_symbols - i;
		}
		freq_[0] = 0;
	}

	void update_model(int symbol) override {
		if (cum_freq_[0] == Max_frequency) {
			auto cum = 0;
			for (auto i = No_of_symbols; i >= 0; i--) {
				freq_[i] = (freq_[i] + 1) / 2;
				cum_freq_[i] = cum;
				cum += freq_[i];
			}
		}

		auto i = symbol;
		for (; freq_[i] == freq_[i - 1]; i--);
		if (i < symbol) {
			auto ch_i = index_to_char_[i];
			auto ch_symbol = index_to_char_[symbol];
			index_to_char_[i] = ch_symbol;
			index_to_char_[symbol] = ch_i;
			char_to_index_[ch_i] = symbol;
			char_to_index_[ch_symbol] = i;
		}
		freq_[i] += 1;
		while (i > 0) {
			i -= 1;
			cum_freq_[i] += 1;
		}
	}
private:
	std::array<int, No_of_symbols + 1> freq_{};
};


class fixed_model : public model {
public:
	fixed_model() : model() {
		cum_freq_[No_of_symbols] = 0;
		for (auto i = No_of_symbols; i > 0; i--) {
			cum_freq_[i - 1] = cum_freq_[i] + freq[i];
		}
	}

	void update_model(int symbol) override {};

private:
	std::array<int, No_of_symbols + 1> freq {
				0,
				1,   1,   1,   1,   1,   1,   1,   1,   1,   1, 124,   1,   1,   1,   1,   1,
				1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,

			 /*      !    *    #    $    %    &    '    (    )    *    +    ,    -    .    / */
				1236,   1,  21,   9,   3,   1,  25,  15,   2,   2,   2,   1,  79,  19,  60,   1,

             /* 0    1    2    3    4    5    6    7    8    9    :    ;    <    =    >    ? */
				15,  15,   8,   5,   4,   7,   5,   4,   4,   6,   3,   2,   1,   1,   1,   1,

			 /* @    A    B    C    D    E    F    G    H    I    J    K    L    M    N    O */
				1,  24,  15,  22,  12,  15,  10,   9,  16,  16,   8,   6,  12,  23,  13,   1,

			 /* P    Q    R    S    T    U    V    W    X    Y    Z    [    \    ]    ^    _ */
				14,   1,  14,  26,  29,   6,   3,  11,   1,   3,   1,   1,   1,   1,   1,   5,

			 /* `    a    b    c    d    e    f    g    h    i    j    k    l    m    n    o */
				1, 491,  85, 173, 232, 744, 127, 110, 293, 418,   6,  39, 250, 139, 429, 446,

			 /* p    q    r    s    t    u    v    w    x    y    z    (    |    )    ~      */
			  111,   5, 388, 375, 531, 159,  57,  91,  12, 101,   5,   2,   1,   2,   3,   1,

				1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
				1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
				1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
				1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
				1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
				1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
				1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
				1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
				1
	};
};


