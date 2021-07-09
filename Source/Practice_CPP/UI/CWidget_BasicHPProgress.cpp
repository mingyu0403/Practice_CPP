#include "CWidget_BasicHPProgress.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCWidget_BasicHPProgress::SetHPProgress(float Percent)
{
	if (HPProgress)
	{
		HPProgress->SetPercent(Percent);
	}
}

void UCWidget_BasicHPProgress::SetHPLabel(FText Text)
{
	if (HPLabel)
	{
		HPLabel->SetText(Text);
	}
}
