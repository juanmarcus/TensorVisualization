#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkVolumeRayCastMapper.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkSphere.h"
#include "vtkSampleFunction.h"
#include "vtkPointData.h"
#include "vtkImageShiftScale.h"
#include "vtkVolumeProperty.h"
#include "vtkPiecewiseFunction.h"
#include "vtkColorTransferFunction.h"
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkCamera.h"
#include "vtkLookupTable.h"

#include "vtkPNrrdReader.h"
#include "vtkImageData.h"

int main()
{
	//  Source
	vtkImageReader2* reader = vtkImageReader2::New();
	reader->SetDataExtent(0, 63, 0, 63, 0, 92);
	reader->SetFileNameSliceOffset(1);
	reader->SetDataScalarTypeToUnsignedShort();
	reader->SetDataByteOrderToLittleEndian();
	reader->SetFilePrefix("/usr/share/VTKData/Data/headsq/quarter");
	reader->SetDataSpacing(3.2, 3.2, 1.5);

	reader->Update();

	reader->GetOutput()->Print(std::cout);

	//	Input data
	vtkPNrrdReader* nrrdReader = vtkPNrrdReader::New();
	nrrdReader->SetFileName("data/A-fa-16.nhdr");
	nrrdReader->Update();

	nrrdReader->Update();
	nrrdReader->GetOutput()->Print(std::cout);

	//  -------------------------------------------------------
	//	Renderer
	vtkRenderer *renderer = vtkRenderer::New();
	renderer->SetBackground(1.0, 1.0, 1.0);

	//	Render window and interactor
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->SetSize(800, 800);
	renWin->AddRenderer(renderer);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	vtkInteractorStyleTrackballCamera *style =
			vtkInteractorStyleTrackballCamera::New();
	iren->SetInteractorStyle(style);

	//	Create a volume
	vtkVolumeRayCastMapper *volumeMapper;
	vtkVolume *volume;

	volumeMapper = vtkVolumeRayCastMapper::New();
	//	volumeMapper->SetBlendModeToComposite(); // composite first
	volumeMapper->SetInputConnection(nrrdReader->GetOutputPort());
	vtkVolumeRayCastCompositeFunction* compositeFunction =
			vtkVolumeRayCastCompositeFunction::New();
	compositeFunction->SetCompositeMethodToInterpolateFirst();
	volumeMapper->SetVolumeRayCastFunction(compositeFunction);

//	vtkLookupTable* volumeColor = vtkLookupTable::New();
//	volumeColor->SetNumberOfTableValues(100);
//	volumeColor->SetTableRange(0, 65535);
//	volumeColor->SetRampToLinear();
//	volumeColor->Build();
//	volumeColor->SetTableValue(0,0,0,0);
//	volumeColor->SetTableValue(99,0,0,1);

	vtkColorTransferFunction* volumeColor = vtkColorTransferFunction::New();
	volumeColor->AddRGBPoint(0, 1,0,0);
	volumeColor->AddRGBPoint(18883, 1,1,0);
	volumeColor->AddRGBPoint(37767, 0,1,0);
	volumeColor->AddRGBPoint(18883+37767, 0,1,1);
	volumeColor->AddRGBPoint(65535, 0,0,1);

	vtkPiecewiseFunction* volumeScalarOpacity = vtkPiecewiseFunction::New();
	volumeScalarOpacity->AddPoint(0, 0.0);
	volumeScalarOpacity->AddPoint(100, 0.01);

	vtkPiecewiseFunction* volumeGradientOpacity = vtkPiecewiseFunction::New();
	volumeGradientOpacity->AddPoint(0, 0.0);
	volumeGradientOpacity->AddPoint(90, 0.5);
	volumeGradientOpacity->AddPoint(100, 1.0);

	vtkVolumeProperty* volumeProperty = vtkVolumeProperty::New();
	volumeProperty->SetColor(volumeColor);
	volumeProperty->SetScalarOpacity(volumeScalarOpacity);
//	volumeProperty->SetGradientOpacity(volumeGradientOpacity);
	volumeProperty->SetInterpolationTypeToLinear();
	volumeProperty->ShadeOn();
//	volumeProperty->SetAmbient(0.6);
//	volumeProperty->SetDiffuse(0.6);
//	volumeProperty->SetSpecular(0.1);

	volume = vtkVolume::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);
	renderer->AddVolume(volume);

	double* c = volume->GetCenter();
	double bounds[6];
	volume->GetBounds(bounds);

	//	vtkCamera* camera = renderer->GetActiveCamera();
	//	camera->SetFocalPoint(c[0], c[1], c[2]);
	//	camera->SetPosition((bounds[1] - bounds[0]) / 2, (bounds[3] - bounds[2])
	//			/ 2, bounds[4] - 100);
	renderer->ResetCamera();

	//	Initialize rendering and interaction

	iren->Initialize();
	iren->Start();

	return 0;
}

