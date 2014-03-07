/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef __elxMultiInputRandomCoordinateSampler_h
#define __elxMultiInputRandomCoordinateSampler_h

#include "itkMultiInputImageRandomCoordinateSampler.h"
#include "elxIncludes.h"

namespace elastix
{


  /**
   * \class MultiInputRandomCoordinateSampler
   * \brief An interpolator based on the itk::MultiInputImageRandomCoordinateSampler.
   *
   * This image sampler randomly samples 'NumberOfSamples' coordinates in
   * the InputImageRegion. If a mask is given, the sampler tries to find
   * samples within the mask. If the mask is very sparse, this may take some time.
   * The MultiInputRandomCoordinate sampler samples not only positions that correspond
   * to voxels, but also positions between voxels. An interpolator for the fixed image is thus
   * required. A B-spline interpolator is used, the order of which can be specified
   * by the user. Typically, the MultiInputRandomCoordinate gives a smoother cost function,
   * because the so-called 'grid-effect' is avoided.
   *
   * Compared to the elx::RandomCoordinateSampler this sampler takes into account the
   * image regions and masks of multiple input images. Thus, only samples are selected that
   * lie within all input images. This is useful when using the
   * MultiResolutionRegistrationWithFeatures.
   *
   * This sampler is suitable to used in combination with the
   * NewSamplesEveryIteration parameter (defined in the elx::OptimizerBase).
   *
   * The parameters used in this class are:
   * \parameter ImageSampler: Select this image sampler as follows:\n
   *    <tt>(ImageSampler "MultiInputRandomCoordinate")</tt>
   * \parameter NumberOfSpatialSamples: The number of image voxels used for computing the
   *    metric value and its derivative in each iteration. Must be given for each resolution.\n
   *    example: <tt>(NumberOfSpatialSamples 2048 2048 4000)</tt> \n
   *    The default is 5000.
   * \parameter UseRandomSampleRegion: Defines whether to randomly select a subregion of the image
   *    in each iteration. When set to "true", also specify the SampleRegionSize.
   *    By setting this option to "true", in combination with the NewSamplesEveryIteration parameter,
   *    a "localised" similarity measure is obtained. This can give better performance in case
   *    of the presence of large inhomogeneities in the image, for example.\n
   *    example: <tt>(UseRandomSampleRegion "true")</tt>\n
   *    Default: false.
   * \parameter SampleRegionSize: the size of the subregions that are selected when using
   *    the UseRandomSampleRegion option. The size should be specified in mm, for each dimension.
   *    As a rule of thumb, you may try a value ~1/3 of the image size.\n
   *    example: <tt>(SampleRegionSize 50.0 50.0 50.0)</tt>\n
   *    You can also specify one number, which will be used for all dimensions. Also, you
   *    can specify different values for each resolution:\n
   *    example: <tt>(SampleRegionSize 50.0 50.0 50.0 30.0 30.0 30.0)</tt>\n
   *    In this example, in the first resolution 50mm is used for each of the 3 dimensions,
   *    and in the second resolution 30mm.\n
   *    Default: sampleRegionSize[i] = min ( fixedImageSize[i], max_i ( fixedImageSize[i]/3 ) ),
   *    with fixedImageSize in mm. So, approximately 1/3 of the fixed image size.
   * \parameter FixedImageBSplineInterpolationOrder: When using a MultiInputRandomCoordinate sampler,
   *    the fixed image needs to be interpolated. This is done using a B-spline interpolator.
   *    With this option you can specify the order of interpolation.\n
   *    example: <tt>(FixedImageBSplineInterpolationOrder 0 0 1)</tt>\n
   *    Default value: 1. The parameter can be specified for each resolution.
   *
   * \ingroup ImageSamplers
   * \sa MultiResolutionRegistrationWithFeatures
   */

  template < class TElastix >
    class MultiInputRandomCoordinateSampler :
    public
      itk::MultiInputImageRandomCoordinateSampler<
      typename elx::ImageSamplerBase<TElastix>::InputImageType >,
    public
      elx::ImageSamplerBase<TElastix>
  {
  public:

    /** Standard ITK-stuff. */
    typedef MultiInputRandomCoordinateSampler        Self;
    typedef itk::MultiInputImageRandomCoordinateSampler<
      typename elx::ImageSamplerBase<TElastix>
      ::InputImageType >                             Superclass1;
    typedef elx::ImageSamplerBase<TElastix>          Superclass2;
    typedef itk::SmartPointer<Self>                  Pointer;
    typedef itk::SmartPointer<const Self>            ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro( MultiInputRandomCoordinateSampler, MultiInputImageRandomCoordinateSampler );

    /** Name of this class.
     * Use this name in the parameter file to select this specific interpolator. \n
     * example: <tt>(ImageSampler "MultiInputRandomCoordinate")</tt>\n
     */
    elxClassNameMacro( "MultiInputRandomCoordinate" );

    /** Typedefs inherited from the superclass. */
    typedef typename Superclass1::DataObjectPointer            DataObjectPointer;
    typedef typename Superclass1::OutputVectorContainerType    OutputVectorContainerType;
    typedef typename Superclass1::OutputVectorContainerPointer OutputVectorContainerPointer;
    typedef typename Superclass1::InputImageType               InputImageType;
    typedef typename Superclass1::InputImagePointer            InputImagePointer;
    typedef typename Superclass1::InputImageConstPointer       InputImageConstPointer;
    typedef typename Superclass1::InputImageRegionType         InputImageRegionType;
    typedef typename Superclass1::InputImagePixelType          InputImagePixelType;
    typedef typename Superclass1::ImageSampleType              ImageSampleType;
    typedef typename Superclass1::ImageSampleContainerType     ImageSampleContainerType;
    typedef typename Superclass1::MaskType                     MaskType;
    typedef typename Superclass1::InputImageIndexType          InputImageIndexType;
    typedef typename Superclass1::InputImagePointType          InputImagePointType;
    typedef typename Superclass1::InputImageSizeType           InputImageSizeType;
    typedef typename Superclass1::InputImageSpacingType        InputImageSpacingType;
    typedef typename Superclass1::InputImagePointValueType     InputImagePointValueType;
    typedef typename Superclass1::ImageSampleValueType         ImageSampleValueType;

    /** This image sampler samples the image on physical coordinates and thus
     * needs an interpolator. */
    typedef typename Superclass1::CoordRepType                 CoordRepType;
    typedef typename Superclass1::InterpolatorType             InterpolatorType;
    typedef typename Superclass1::DefaultInterpolatorType      DefaultInterpolatorType;

    /** The input image dimension. */
    itkStaticConstMacro( InputImageDimension, unsigned int, Superclass1::InputImageDimension );

    /** Typedefs inherited from Elastix. */
    typedef typename Superclass2::ElastixType               ElastixType;
    typedef typename Superclass2::ElastixPointer            ElastixPointer;
    typedef typename Superclass2::ConfigurationType         ConfigurationType;
    typedef typename Superclass2::ConfigurationPointer      ConfigurationPointer;
    typedef typename Superclass2::RegistrationType          RegistrationType;
    typedef typename Superclass2::RegistrationPointer       RegistrationPointer;
    typedef typename Superclass2::ITKBaseType               ITKBaseType;

    /** Execute stuff before each resolution:
     * \li Set the number of samples.
     * \li Set the fixed image interpolation order
     * \li Set the UseRandomSampleRegion flag and the SampleRegionSize
     */
    virtual void BeforeEachResolution(void);

  protected:

    /** The constructor. */
    MultiInputRandomCoordinateSampler() {}
    /** The destructor. */
    virtual ~MultiInputRandomCoordinateSampler() {}

  private:

    /** The private constructor. */
    MultiInputRandomCoordinateSampler( const Self& ); // purposely not implemented
    /** The private copy constructor. */
    void operator=( const Self& );      // purposely not implemented

  }; // end class MultiInputRandomCoordinateSampler


} // end namespace elastix

#ifndef ITK_MANUAL_INSTANTIATION
#include "elxMultiInputRandomCoordinateSampler.hxx"
#endif

#endif // end #ifndef __elxMultiInputRandomCoordinateSampler_h
