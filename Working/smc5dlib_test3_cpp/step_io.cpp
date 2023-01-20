#include "pch.h"
#include "step_io.h"

static int reverse_endian(int value);

StepList^ StepIO::generate_square(float minX, float minY, float maxX, float maxY, float height, float speed, SMCSettings^ smc_settings)
{
    StepList^ stepList = gcnew StepList(smc_settings);

    stepList->AddXYZ(minX, minY, height, speed);
    stepList->AddXYZ(maxX, minY, height, speed);
    stepList->AddXYZ(maxX, maxY, height, speed);
    stepList->AddXYZ(minX, maxY, height, speed);
    stepList->AddXYZ(minX, minY, height, speed);

    return stepList;
}
StepList^ StepIO::generate_line(float startX, float startY, float endX, float endY, float height, float speed, SMCSettings^ smc_settings)
{
    StepList^ stepList = gcnew StepList(smc_settings);

    stepList->AddXYZ(startX, startY, height, speed);
    stepList->AddXYZ(endX, endY, height, speed);

    return stepList;
}
StepList^ StepIO::generate_circle(float centerX, float centerY, float radius, float height, float speed, unsigned int divisions, SMCSettings^ smc_settings)
{
    StepList^ stepList = gcnew StepList(smc_settings);

    stepList->AddXYZ(centerX + radius, centerY, height, 50.0f);
    for (unsigned int i = 1; i < divisions; i++)
    {
        float radiant = (i * TWO_PI) / divisions;
        stepList->AddXYZ(centerX + radius * cos(radiant), centerY + radius * sin(radiant), height, speed);
    }
    stepList->AddXYZ(centerX + radius, centerY, height, speed);

    return stepList;
}
StepList^ StepIO::load_grf4(System::String^ file_path, float height, float speed, SMCSettings^ smc_settings)
{
    // Quite specific to cncgraf.
    XmlTextReader^ reader = gcnew XmlTextReader(file_path);
    StepList^ stepList = gcnew StepList(smc_settings);

    while (reader->Read())
    {
        if (reader->Name->Equals("Polygon"))
        {
            reader->Read(); // Read in the next node.
            if (reader->Value->Length <= 8) continue;   // Skip the empty lines.
            // TODO: Be careful of endianess. Needs rework when Refactoring!!
            String^ str = reader->Value->Replace(" ", "");

            // Read x-value.
            String^ hex = str->Substring(0, 8);
            unsigned int num = reverse_endian(UInt32::Parse(hex, System::Globalization::NumberStyles::AllowHexSpecifier));
            array<unsigned char>^ floatVals = BitConverter::GetBytes(num);
            float x = BitConverter::ToSingle(floatVals, 0);
            // Read y_value
            hex = str->Substring(8, 8);
            num = reverse_endian(UInt32::Parse(hex, System::Globalization::NumberStyles::AllowHexSpecifier));
            floatVals = BitConverter::GetBytes(num);
            float y = BitConverter::ToSingle(floatVals, 0);
            // Add step to the step list.
            stepList->AddXYZ(x, y, height, speed);
        }
    }
    reader->Close();
    return stepList;
}

StepList^ StepIO::load_binary(System::String^ file_path, SMCSettings^ smc_settings)
{
    StepReader^ step_reader = gcnew StepReader(file_path);
    if (step_reader == nullptr) return nullptr;
    StepList^ step_list = gcnew StepList(smc_settings);
    step_list->List = step_reader->ReadToEnd();
    step_reader->Close();
    return step_list;
}
void StepIO::write_binary(System::String^ file_path, StepList^ step_list)
{
    StepWriter^ myStepWriter = gcnew StepWriter(file_path);
    if (myStepWriter == nullptr) return;
    myStepWriter->Write(step_list->List);
    myStepWriter->Close();
}

static int reverse_endian(int value)
{
    byte* p = (byte*)&value;
    return (*p << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}