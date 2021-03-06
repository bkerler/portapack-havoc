/*
 * Copyright (C) 2015 Jared Boone, ShareBrained Technology, Inc.
 * Copyright (C) 2016 Furrtek
 *
 * This file is part of PortaPack.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include "ui.hpp"
#include "ui_menu.hpp"
#include "ui_navigation.hpp"
#include "ui_font_fixed_8x16.hpp"
#include "ui_receiver.hpp"
#include "ui_transmitter.hpp"
#include "ui_textentry.hpp"
#include "message.hpp"
#include "rds.hpp"

using namespace rds;

namespace ui {

class RDSView : public View {
public:
	RDSView(NavigationView& nav);
	~RDSView();

	void focus() override;
	void paint(Painter& painter) override;

	std::string title() const override { return "RDS transmit"; };

private:
	std::string PSN { 0 };
	std::string RadioText { 0 };
	bool txing = false;
	RDS_flags rds_flags { };
	
	uint16_t message_length { 0 };
	
	void start_tx();
	void on_tuning_frequency_changed(rf::Frequency f);

	Labels labels {
		{ { 1 * 8, 16 + 8 }, "PTY:", Color::light_grey() },
		{ { 14 * 8, 16 + 8 }, "CC:", Color::light_grey() },
		{ { 1 * 8, 32 + 8 }, "PI:", Color::light_grey() },
		{ { 13 * 8, 32 + 8 }, "Cov:", Color::light_grey() },
		{ { 2 * 8, 13 * 8 }, "PSN:", Color::light_grey() },
		{ { 2 * 8, 8 * 16 }, "RadioText:", Color::light_grey() },
		{ { 2 * 8, 14 * 16 }, "TX group:", Color::light_grey() }
	};
	
	OptionsField options_pty {
		{ 5 * 8, 16 + 8 },
		8,
		{
			{ "None", 0 },
			{ "News", 1 },
			{ "Affairs", 2 },
			{ "Info", 3 },
			{ "Sport", 4 },
			{ "Educate", 5 },
			{ "Drama", 6 },
			{ "Culture", 7 },
			{ "Science", 8 },
			{ "Varied", 9 },
			{ "Pop", 10 },
			{ "Rock", 11 },
			{ "Easy", 12 },
			{ "Light", 13 },
			{ "Classics", 14 },
			{ "Other", 15 },
			{ "Weather", 16 },
			{ "Finance", 17 },
			{ "Children", 18 },
			{ "Social", 19 },
			{ "Religion", 20 },
			{ "PhoneIn", 21 },
			{ "Travel", 22 },
			{ "Leisure", 23 },
			{ "Jazz", 24 },
			{ "Country", 25 },
			{ "National", 26 },
			{ "Oldies", 27 },
			{ "Folk", 28 },
			{ "Docs", 29 },
			{ "AlarmTst", 30 },
			{ "Alarm", 31 }
		}
	};
	
	OptionsField options_countrycode {
		{ 17 * 8, 16 + 8 },
		11,
		{
			{ "Albania", 	9 },
			{ "Algeria", 	2 },
			{ "Andorra", 	3 },
			{ "Austria", 	10 },
			{ "Azores", 	8 },
			{ "Belgium", 	6 },
			{ "Belarus", 	15 },
			{ "Bosnia", 	15 },
			{ "Bulgaria", 	8 },
			{ "Canaries", 	14 },
			{ "Croatia", 	12 },
			{ "Cyprus", 	2 },
			{ "Czech-Rep", 	2 },
			{ "Denmark", 	9 },
			{ "Egypt", 		15 },
			{ "Estonia", 	2 },
			{ "Faroe", 		9 },
			{ "Finland", 	6 },
			{ "France", 	15 },
			{ "Germany 1", 	13 },
			{ "Germany 2", 	1 },
			{ "Gibraltar", 	10 },
			{ "Greece", 	1 },
			{ "Hungary", 	11 },
			{ "Iceland", 	10 },
			{ "Iraq", 		11 },
			{ "Ireland", 	2 },
			{ "Israel", 	4 },
			{ "Italy", 		5 },
			{ "Jordan", 	5 },
			{ "Latvia", 	9 },
			{ "Lebanon", 	10 },
			{ "Libya", 		13 },
			{ "Liechtenst.", 9 },
			{ "Lithuania", 	12 },
			{ "Luxembourg", 7 },
			{ "Macedonia", 	4 },
			{ "Madeira", 	8 },
			{ "Malta", 		12 },
			{ "Moldova", 	1 },
			{ "Monaco", 	11 },
			{ "Morocco", 	1 },
			{ "Netherlands", 8 },
			{ "Norway", 	15 },
			{ "Palestine", 	8 },
			{ "Poland", 	3 },
			{ "Portugal", 	8 },
			{ "Romania", 	14 },
			{ "Russia", 	7 },
			{ "San Marino", 3 },
			{ "Slovakia", 	5 },
			{ "Slovenia", 	9 },
			{ "Spain", 		14 },
			{ "Sweden", 	14 },
			{ "Switzerland", 4 },
			{ "Syria", 		6 },
			{ "Tunisia", 	7 },
			{ "Turkey", 	3 },
			{ "Ukraine", 	6 },
			{ "U.K.", 		12 },
			{ "Vatican", 	4 },
			{ "Yugoslavia", 13 }
		}
	};
	
	SymField sym_pi_code {
		{ 4 * 8, 32 + 8 },
		4,
		SymField::SYMFIELD_HEX
	};
	
	OptionsField options_coverage {
		{ 17 * 8, 32 + 8 },
		12,
		{
			{ "Local", 0 },
			{ "Internat.", 1 },
			{ "National", 2 },
			{ "Sup-regional", 3 },
			{ "R11", 4 },
			{ "R12", 5 },
			{ "R13", 6 },
			{ "R14", 7 },
			{ "R15", 8 },
			{ "R16", 9 },
			{ "R17", 10 },
			{ "R18", 11 },
			{ "R19", 12 },
			{ "R110", 13 },
			{ "R111", 14 },
			{ "R112", 15 }
		}
	};

	Checkbox check_mono_stereo {
		{ 1 * 8, 4 * 16 },
		6,
		"Stereo"
	};
	Checkbox check_TA {
		{ 12 * 8, 4 * 16 },
		2,
		"TA"
	};
	Checkbox check_TP {
		{ 18 * 8, 4 * 16 },
		2,
		"TP"
	};
	Checkbox check_MS {
		{ 24 * 8, 4 * 16 },
		2,
		"MS"
	};

	Button button_editpsn {
		{ 22 * 8, 5 * 16 + 20, 48, 24 },
		"Set"
	};
	Text text_psn {
		 { 6 * 8, 13 * 8, 4 * 8, 16 },
		 ""
	};

	Text text_radiotexta {
		{ 2 * 8, 9 * 16, 19 * 8, 16 },
		"-"
	};
	Text text_radiotextb {
		{ 2 * 8, 10 * 16, 19 * 8, 16 },
		"-"
	};
	Button button_editradiotext {
		{ 22 * 8, 8 * 16 + 12, 48, 24 },
		"Set"
	};
	
	OptionsField options_tx {
		{ 11 * 8, 14 * 16 },
		11,
		{
			{ "PSN", 0 },
			{ "RadioText", 1 },
			{ "Date & time", 2 }
		}
	};
	
	TransmitterView tx_view {
		16 * 16,
		50000,
		9
	};
};

} /* namespace ui */
