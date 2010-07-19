#include <vtkSphereSource.h>

#include <vtkPolyDataMapper.h>
#include <vtkArrowSource.h>
#include <vtkGlyph2D.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderer.h>
#include <vtkImageGradient.h>
#include <vtkImageData.h>

int main(int, char *[])
{
	// Create a scalar data container
	vtkImageData* imageData = vtkImageData::New();
	imageData->SetScalarTypeToFloat();
	imageData->SetExtent(0, 15, 0, 15, 0, 0);
	imageData->SetNumberOfScalarComponents(1);
	imageData->AllocateScalars();

	int dims[3];
	imageData->GetDimensions(dims);
	//	std::cout << dims[0] << "," << dims[1] << "," << dims[2] << std::endl;

	for (int z = 0; z < dims[2]; z++)
	{
		for (int y = 0; y < dims[1]; y++)
		{
			for (int x = 0; x < dims[0]; x++)
			{
				float* pixel =
						static_cast<float*> (imageData->GetScalarPointer(x, y,
								z));
				pixel[0] = ((float) (x * y)) / ((float) (dims[0] * dims[1]));
			}
		}
	}

	// Compute gradients
	vtkImageGradient* imageGradient = vtkImageGradient::New();
	imageGradient->HandleBoundariesOn();
	imageGradient->SetInput(imageData);

	// Create the glyph source
	vtkArrowSource* arrowSource = vtkArrowSource::New();

	// Create glyphs
	vtkGlyph2D* glyph2D = vtkGlyph2D::New();
	glyph2D->SetSourceConnection(arrowSource->GetOutputPort());
	glyph2D->SetInputConnection(imageGradient->GetOutputPort());
	glyph2D->ScalingOff();
	glyph2D->OrientOn();

	// Create the Renderer
	vtkRenderer* renderer = vtkRenderer::New();
	renderer->SetBackground(1, 1, 1);

	// Create the RenderWindow
	vtkRenderWindow* renderWindow = vtkRenderWindow::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(1024, 768);

	// Create the RenderWindowInteractor
	vtkRenderWindowInteractor* renderWindowInteractor =
			vtkRenderWindowInteractor::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	// Create the Interactor Style
	vtkInteractorStyleTrackballCamera* style =
			vtkInteractorStyleTrackballCamera::New();
	renderWindowInteractor->SetInteractorStyle(style);

	// Create actors and mappers
	vtkPolyDataMapper* glyphMapper = vtkPolyDataMapper::New();
	glyphMapper->SetInputConnection(glyph2D->GetOutputPort());
	vtkActor* glyphActor = vtkActor::New();
	glyphActor->SetMapper(glyphMapper);
	renderer->AddActor2D(glyphActor);

	// Start interaction
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}
