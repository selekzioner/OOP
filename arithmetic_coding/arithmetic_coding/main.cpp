#include "arithmetic_coder.hpp"
#include "model.hpp"


int encoding(std::string&& in_filename, std::string&& out_filename, 
        std::shared_ptr<model> model = std::make_shared<adaptive_model>()) {

    std::ifstream in_stream(in_filename, std::ios_base::binary);
	if (!in_stream) {
		std::cout << "can't open input file: " << in_filename << std::endl;
		return -1;
	}

    obitstream out_stream(out_filename);
    if (!out_stream) {
		std::cout << "can't open input file: " << out_filename << std::endl;
		return -1;
    }

    arithmetic_encoder encoder(out_stream);
    encoder.set_model(model);
    encoder.encode(in_stream);
	return 0;
}


int decoding(std::string&& in_filename, std::string&& out_filename, 
        std::shared_ptr<model> model = std::make_shared<adaptive_model>()) {

    std::ofstream out_stream(out_filename, std::ios_base::binary);
	if (!out_stream) {
		std::cout << "can't open input file: " << out_filename << std::endl;
		return -1;
	}

    ibitstream in_stream(in_filename);
	if (!in_stream) {
		std::cout << "can't open input file: " << in_filename << std::endl;
		return -1;
	}

    arithmetic_decoder decoder(in_stream);
    decoder.set_model(model);
	try {
		decoder.decode(out_stream);
	}
	catch (std::ios_base::failure& ex) {
		std::cout << ex.what() << std::endl;
		return -1;
	}
	return 0;
}


void show_interface() {
	std::cout << "arithmetic_coding [MODE] [MODEL] [INPUT FILE] [OUTPUT FILE]\n" << std::endl;
	std::cout << "[MODE]=encode:\n\t-e, --encode\n\t-d, --decode\n" << std::endl;
	std::cout << "[MODEL]=adaptive:\n\t-a, --adaptive\n\t-f, --fixed\n" << std::endl;
}


int main(int argc, char* argv[]) {
	switch (argc) {
		case 3: {
			return encoding(argv[1], argv[2]);
		}
		case 4: {
			if (!strcmp(argv[1], "-d")) {
				return decoding(argv[2], argv[3]);
			}
			if (!strcmp(argv[1], "-f")) {
				return encoding(argv[2], argv[3], std::make_shared<fixed_model>());
			}
			if (!strcmp(argv[1], "-e") || !strcmp(argv[1], "-a")) {
				return encoding(argv[2], argv[3]);
			}
		}
		case 5: {
			std::shared_ptr<model> mod;
			if (!strcmp(argv[2], "-f")) {
				mod = std::make_shared<fixed_model>();
			}
			else if (!strcmp(argv[2], "-a")) {
				mod = std::make_shared<adaptive_model>();
			}
			if (!strcmp(argv[1], "-e")) {
				return encoding(argv[3], argv[4], mod);
			}
			if (!strcmp(argv[1], "-d")) {
				return decoding(argv[3], argv[4], mod);
			}
		}
	}
	show_interface();
	return 0;
}