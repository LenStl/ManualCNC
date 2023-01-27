#pragma once
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
	 * @param minX - Minimum and starting x coordinate of the square.
	 * @param minY - Minimum and starting y coordinate of the square.
	 * @param maxX - Maximum x coordinate of the square.
	 * @param maxY - Maximum y coordinate of the square.
	 * @param height - Constant traveling height at which the head moves.
	 * @param speed - Constant speed at which the head moves between steps.
	 * @param smcSettings - SMCSettings of the targeted connector.
	 * @return A StepList^ of steps representing a square.
	 */
	inline StepList^ generateSquare(float minX, float minY, float maxX, float maxY, float height, float speed, SMCSettings^ smcSettings)
	{
		StepList^ stepList = gcnew StepList(smcSettings);

		stepList->AddXYZ(minX, minY, height, INIT_SPEED);
		stepList->AddXYZ(maxX, minY, height, speed);
		stepList->AddXYZ(maxX, maxY, height, speed);
		stepList->AddXYZ(minX, maxY, height, speed);
		stepList->AddXYZ(minX, minY, height, speed);

		return stepList;
	}

	/**
	 * Generates a list of steps for the SMC5D controller to linearly move along a line.
	 *
	 * @param startX - Starting x coordinate of the line.
	 * @param startY - Starting y coordinate of the line.
	 * @param endX - End x coordinate of the line.
	 * @param endY - End y coordinate of the line.
	 * @param height - Constant traveling height at which the head moves.
	 * @param speed - Constant speed at which the head moves between steps.
	 * @param smcSettings - SMCSettings of the targeted connector.
	 * @return A StepList^ of steps representing a line.
	 */
	inline StepList^ generateLine(float startX, float startY, float endX, float endY, float height, float speed, SMCSettings^ smcSettings)
	{
		StepList^ stepList = gcnew StepList(smcSettings);

		stepList->AddXYZ(startX, startY, height, INIT_SPEED);
		stepList->AddXYZ(endX, endY, height, speed);

		return stepList;
	}

	/**
	 * Generates a list of steps for the SMC5D controller to move along a circle.
	 *
	 * @param centerX - X coordinate of the center of the circle.
	 * @param centerY - Y coordinate of the center of the circle.
	 * @param radius - Radius of the circle.
	 * @param height - Constant traveling height at which the head moves.
	 * @param speed - Constant speed at which the head moves between steps.
	 * @param divisions - Number of linear segments.
	 * @param smcSettings - SMCSettings of the targeted connector.
	 * @return A StepList^ of steps representing a circle.
	 */
	inline StepList^ generateCircle(float centerX, float centerY, float radius, float height, float speed, unsigned int divisions, SMCSettings^ smcSettings)
	{
		StepList^ stepList = gcnew StepList(smcSettings);

		stepList->AddXYZ(centerX + radius, centerY, height, INIT_SPEED);
		for (unsigned int i = 1; i < divisions; i++)
		{
			float radiant = (i * TWO_PI) / divisions;
			stepList->AddXYZ(centerX + radius * cos(radiant), centerY + radius * sin(radiant), height, speed);
		}
		stepList->AddXYZ(centerX + radius, centerY, height, speed);

		return stepList;
	}

	/**
	 * Reads the polygon coordinates from a .grf4 or .grf5 file and converts them to a list of steps.
	 *
	 * @param filePath - Path to a .grf4 or .grf5 file with ordered segment coordinates on a curve.
	 * @param height - Constant traveling height at which the head moves.
	 * @param speed - Constant speed at which the head moves between steps.
	 * @param smcSettings - SMCSettings of the targeted connector.
	 * @return A StepList^ of linearly interpolated steps representing the curve defined in file_path.
	 */
	inline StepList^ loadGRF(System::String^ filePath, float height, float speed, SMCSettings^ smcSettings)
	{
		// Quite specific to cncGraF.
		XmlTextReader^ reader = gcnew XmlTextReader(filePath);
		StepList^ stepList = gcnew StepList(smcSettings);

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
				stepList->AddXYZ(x, y, height, speed);
			}
		}
		reader->Close();
		return stepList;
	}

	// .sl (Step List) => File format generated by StepWriter::Write(). NOTE: .sl solely exists in this libraries context.
	/**
	 * Reads the coordinates from a binary .sl (Step List) file and converts them to a list of steps.
	 *
	 * @param filePath - Path to a .sl (Step List) file with ordered segment coordinates on a curve.
	 * @param smcSettings - SMCSettings of the targeted connector.
	 * @return A StepList^ of linearly interpolated steps representing the curve defined in file_path.
	 */
	inline StepList^ loadBinary(System::String^ filePath, SMCSettings^ smcSettings)
	{
		StepReader^ stepReader = gcnew StepReader(filePath);
		if (stepReader == nullptr) return nullptr;
		StepList^ stepList = gcnew StepList(smcSettings);
		stepList->List = stepReader->ReadToEnd();
		stepReader->Close();
		return stepList;
	}

	/**
	 * Writes the coordinates from a list of steps to file_path in binary.
	 *
	 * @param filePath - Path to a .sl (Step List) file to write to.
	 * @param stepList - The list of steps that should be written.
	 * @return 0 for success, 1 for error.
	 */
	inline int writeBinary(System::String^ filePath, StepList^ stepList)
	{
		StepWriter^ stepWriter = gcnew StepWriter(filePath);
		if (stepWriter == nullptr) return 1;
		stepWriter->Write(stepList->List);
		stepWriter->Close();
		return 0;
	}
}

static int reverseEndian(int value)
{
	// Reversing the byte-order of value.
	byte* p = (byte*)&value;
	return (*p << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}
