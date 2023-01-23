#include "pch.h"
#include "step_io.h"

static int reverseEndian(int value);

StepList^ StepIO::generateSquare(float minX, float minY, float maxX, float maxY, float height, float speed, SMCSettings^ smc_settings)
{
    StepList^ step_list = gcnew StepList(smc_settings);

    step_list->AddXYZ(minX, minY, height, speed);
    step_list->AddXYZ(maxX, minY, height, speed);
    step_list->AddXYZ(maxX, maxY, height, speed);
    step_list->AddXYZ(minX, maxY, height, speed);
    step_list->AddXYZ(minX, minY, height, speed);

    return step_list;
}
StepList^ StepIO::generateLine(float startX, float startY, float endX, float endY, float height, float speed, SMCSettings^ smc_settings)
{
    StepList^ step_list = gcnew StepList(smc_settings);

    step_list->AddXYZ(startX, startY, height, speed);
    step_list->AddXYZ(endX, endY, height, speed);

    return step_list;
}
StepList^ StepIO::generateCircle(float centerX, float centerY, float radius, float height, float speed, unsigned int divisions, SMCSettings^ smc_settings)
{
    StepList^ step_list = gcnew StepList(smc_settings);

    step_list->AddXYZ(centerX + radius, centerY, height, 50.0f);
    for (unsigned int i = 1; i < divisions; i++)
    {
        float radiant = (i * TWO_PI) / divisions;
        step_list->AddXYZ(centerX + radius * cos(radiant), centerY + radius * sin(radiant), height, speed);
    }
    step_list->AddXYZ(centerX + radius, centerY, height, speed);

    return step_list;
}
StepList^ StepIO::loadGRF(System::String^ file_path, float height, float speed, SMCSettings^ smc_settings)
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

StepList^ StepIO::loadBinary(System::String^ file_path, SMCSettings^ smc_settings)
{
    StepReader^ step_reader = gcnew StepReader(file_path);
    if (step_reader == nullptr) return nullptr;
    StepList^ step_list = gcnew StepList(smc_settings);
    step_list->List = step_reader->ReadToEnd();
    step_reader->Close();
    return step_list;
}
void StepIO::writeBinary(System::String^ file_path, StepList^ step_list)
{
    StepWriter^ step_writer = gcnew StepWriter(file_path);
    if (step_writer == nullptr) return;
    step_writer->Write(step_list->List);
    step_writer->Close();
}

static int reverseEndian(int value)
{
    byte* p = (byte*)&value;
    return (*p << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}