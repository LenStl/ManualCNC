#pragma once
#include "pch.h"
#include <cstdio>
#include <windows.h>
using namespace System;
using namespace De::Boenigk::SMC5D::Settings;
using namespace De::Boenigk::SMC5D::Basics;
using namespace De::Boenigk::SMC5D::Move;

using namespace System::Xml;

static int reverseEndian(int value);

namespace StepIO
{
	const float TWO_PI = 6.2831855f;
	const float INIT_SPEED = 40.0f;
	/**
	 * Generates a list of steps for the SMC5D controller to move along a 2D-AABB.
	 *
	 * @param min_x - Minimum and starting x coordinate of the square.
	 * @param min_y - Minimum and starting y coordinate of the square.
	 * @param max_x - Maximum x coordinate of the square.
	 * @param max_y - Maximum y coordinate of the square.
	 * @param height - Constant traveling height at which the head moves.
	 * @param speed - Constant speed at which the head moves between steps.
	 * @param smc_settings - SMCSettings of the targeted connector.
	 * @return A StepList^ of steps representing a square.
	 */
	StepList^ generateSquare(float min_x, float min_y, float max_x, float max_y, float height, float speed, SMCSettings^ smc_settings)
	{
		StepList^ step_list = gcnew StepList(smc_settings);

		step_list->AddXYZ(min_x, min_y, height, INIT_SPEED);
		step_list->AddXYZ(max_x, min_y, height, speed);
		step_list->AddXYZ(max_x, max_y, height, speed);
		step_list->AddXYZ(min_x, max_y, height, speed);
		step_list->AddXYZ(min_x, min_y, height, speed);

		return step_list;
	}

	/**
	 * Generates a list of steps for the SMC5D controller to linearly move along a line.
	 *
	 * @param start_x - Starting x coordinate of the line.
	 * @param start_y - Starting y coordinate of the line.
	 * @param end_x - End x coordinate of the line.
	 * @param end_y - End y coordinate of the line.
	 * @param height - Constant traveling height at which the head moves.
	 * @param speed - Constant speed at which the head moves between steps.
	 * @param smc_settings - SMCSettings of the targeted connector.
	 * @return A StepList^ of steps representing a line.
	 */
	StepList^ generateLine(float start_x, float start_y, float end_x, float end_y, float height, float speed, SMCSettings^ smc_settings)
	{
		StepList^ step_list = gcnew StepList(smc_settings);

		step_list->AddXYZ(start_x, start_y, height, INIT_SPEED);
		step_list->AddXYZ(end_x, end_y, height, speed);

		return step_list;
	}
	/**
	 * Generates a list of steps for the SMC5D controller to move along a circle.
	 *
	 * @param center_x - X coordinate of the center of the circle.
	 * @param center_y - Y coordinate of the center of the circle.
	 * @param radius - Radius of the circle.
	 * @param height - Constant traveling height at which the head moves.
	 * @param speed - Constant speed at which the head moves between steps.
	 * @param divisions - Number of linear segments.
	 * @param smc_settings - SMCSettings of the targeted connector.
	 * @return A StepList^ of steps representing a circle.
	 */
	StepList^ generateCircle(float center_x, float center_y, float radius, float height, float speed, unsigned int divisions, SMCSettings^ smc_settings)
	{
		StepList^ step_list = gcnew StepList(smc_settings);

		step_list->AddXYZ(center_x + radius, center_y, height, INIT_SPEED);
		for (unsigned int i = 1; i < divisions; i++)
		{
			float radiant = (i * TWO_PI) / divisions;
			step_list->AddXYZ(center_x + radius * cos(radiant), center_y + radius * sin(radiant), height, speed);
		}
		step_list->AddXYZ(center_x + radius, center_y, height, speed);

		return step_list;
	}

	/**
	 * Reads the polygon coordinates from a .grf4 or .grf5 file and converts them to a list of steps.
	 *
	 * @param file_path - Path to a .grf4 or .grf5 file with ordered segment coordinates on a curve.
	 * @param height - Constant traveling height at which the head moves.
	 * @param speed - Constant speed at which the head moves between steps.
	 * @param smc_settings - SMCSettings of the targeted connector.
	 * @return A StepList^ of linearly interpolated steps representing the curve defined in file_path.
	 */
	StepList^ loadGRF(System::String^ file_path, float height, float speed, SMCSettings^ smc_settings)
	{
		// Quite specific to cncGraF.
		XmlTextReader^ reader = gcnew XmlTextReader(file_path);
		StepList^ step_list = gcnew StepList(smc_settings);

		while (reader->Read())
		{
			if (reader->Name->Equals("Polygon"))
			{
				reader->Read(); // Read in the next node.
				if (reader->Value->Length <= 8) continue;   // Skip the empty lines.
				String^ str = reader->Value->Replace(" ", "");

				// Read x-value.
				String^ hex = str->Substring(0, 8);
				unsigned int num = reverseEndian(UInt32::Parse(hex, System::Globalization::NumberStyles::AllowHexSpecifier));
				array<unsigned char>^ float_vals = BitConverter::GetBytes(num);
				float x = BitConverter::ToSingle(float_vals, 0);
				// Read y_value
				hex = str->Substring(8, 8);
				num = reverseEndian(UInt32::Parse(hex, System::Globalization::NumberStyles::AllowHexSpecifier));
				float_vals = BitConverter::GetBytes(num);
				float y = BitConverter::ToSingle(float_vals, 0);

				// Add step to the step list.
				step_list->AddXYZ(x, y, height, speed);
			}
		}
		reader->Close();
		return step_list;
	}
	// .sl (Step List) => File format generated by StepWriter::Write(). NOTE: .sl solely exists in this libraries context.
	/**
	 * Reads the coordinates from a binary .sl (Step List) file and converts them to a list of steps.
	 *
	 * @param file_path - Path to a .sl (Step List) file with ordered segment coordinates on a curve.
	 * @param smc_settings - SMCSettings of the targeted connector.
	 * @return A StepList^ of linearly interpolated steps representing the curve defined in file_path.
	 */
	StepList^ loadBinary(System::String^ file_path, SMCSettings^ smc_settings)
	{
		StepReader^ step_reader = gcnew StepReader(file_path);
		if (step_reader == nullptr) return nullptr;
		StepList^ step_list = gcnew StepList(smc_settings);
		step_list->List = step_reader->ReadToEnd();
		step_reader->Close();
		return step_list;
	}
	/**
	 * Writes the coordinates from a list of steps to file_path in binary.
	 *
	 * @param file_path - Path to a .sl (Step List) file to write to.
	 * @param step_list - The list of steps that should be written.
	 * @return 0 for success, 1 for error.
	 */
	int writeBinary(System::String^ file_path, StepList^ step_list)
	{
		StepWriter^ step_writer = gcnew StepWriter(file_path);
		if (step_writer == nullptr) return 1;
		step_writer->Write(step_list->List);
		step_writer->Close();
		return 0;
	}
}

// Reversing the byte-order of value.
static int reverseEndian(int value)
{
	byte* p = (byte*)&value;
	return (*p << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}
