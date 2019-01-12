#!/usr/bin/perl
use strict;
use warnings;
use POSIX qw(strftime);
use Cwd;

my $author = "Roberto Brown";
my $copyright = "\@copyright ";
my @namespaces;
my $class_tested;
my $class_name;
my $path = cwd();
my $date = strftime "%m/%d/%Y", localtime;
my $outfile;
my $interface_class = "";

sub write_h_interface_file;
sub write_h_mock_file;
sub write_h_file;
sub write_cpp_file;
sub write_test_file;
sub println;
sub usage;

#############################################################################
# input: namespace::namespace::class (I_class = interface, class_test = test)
sub main
{
  (@namespaces) = split m%::%, $_[0];

  $class_name = pop(@namespaces);
  $path =~ s/^.*\/(src|Source)\///;

  if ( $class_name =~ /_test$/ )
  {
    write_test_file();
  }
  elsif ( $class_name =~ /^I_/ )
  {
    $class_name =~ s/^I_//;
    write_h_interface_file();
  }
  elsif ( $class_name =~ /^Mock_/ )
  {
    write_h_mock_file();
  }
  elsif ( $class_name =~ /\.h$/ )
  {
    $class_name =~ s/\.h$//;
    write_h_file();
  }
  elsif ( $class_name =~ /\.cpp$/ )
  {
    $class_name =~ s/\.cpp$//;
    write_cpp_file();
  }
  else
  {
    write_h_file();
    write_cpp_file();
  }
}

#############################################################################
#
sub usage()
{
  print STDERR "Usage: [namespace::]class_name;\n";
  exit;
}

#############################################################################
#
sub println
{
  print {$outfile} @_, "\n";
}

#############################################################################
#
sub print_level
{
  my ($level, @rest) = @_;

  for ( my $n = 0; $n < $level; $n++ )
  {
    print {$outfile} "  ";
  }

  println @rest;
}

#############################################################################
#
sub write_method_header
{
  print {$outfile} "//";
  for (my $n = 0; $n < 118; $n++ )
  {
    print {$outfile} "=";
  }

  println;
  println "//";
}

#############################################################################
#
sub write_h_file_header
{
  println "///";
  println "/// \@file $path/$class_name\.h";
  println "///";
  #println "/// \@author $author";
  #println "///";
  #println "/// \@date $date: $author -- Initial release.";
  #println "///";
  println "/// This file contains the $class_name class definition.";
  println "///";
  #println "/// $copyright";
  #println "///";
  println;
}

#############################################################################
#
sub write_cpp_file_header
{
  println "///";
  println "/// \@file $path/$class_name\.cpp";
  println "///";
  #println "/// \@author $author";
  #println "///";
  #println "/// \@date $date: $author -- Initial release.";
  #println "///";
  println "/// This file contains the $class_name class implementation.";
  println "///";
  #println "/// $copyright";
  #println "///";
  println;
}

#############################################################################
#
sub write_test_file_header
{
  println "///";
  println "/// \@file $path/$class_name\.cpp";
  #println "///";
  #println "/// \@author $author";
  #println "///";
  #println "/// \@date $date: $author -- Initial release.";
  println "///";
  println "/// This file contains the $class_tested class tests.";
  println "///";
  #println "/// $copyright";
  #println "///";
  println;
}

#############################################################################
#
sub create_header_guard
{
  my $header_guard;
  foreach my $namespace (@namespaces)
  {
    $header_guard .= uc($namespace);
    $header_guard .= "_";
  }

  $header_guard .= uc($class_name)."_H";
  return $header_guard;
}

#############################################################################
#
sub write_h_file_body
{
  my $level = 0;

  my $header_guard = create_header_guard();

  println "#ifndef ",$header_guard;
  println "#define ",$header_guard;
  println;

  #println "#include <Cps/Lib/System/Error.h>";
  #println;

  foreach my $namespace (@namespaces)
  {
    print_level $level, "namespace $namespace";
    print_level $level, "{";
  }

  println;
  print_level $level, "/// class description.";
  print_level $level, "class $class_name";
  print_level $level, "{";
  print_level $level, "public:";
  print_level $level, "  /// Default object constructor.";
  print_level $level, "  $class_name();";
  println;
  print_level $level, "  /// Object destructor.";
  print_level $level, "  virtual ~$class_name();";
  println;
  # print_level $level, "  /// Object initialization function.";
  # print_level $level, "  ///";
  # print_level $level, "  /// \@return zero on success.";
  # print_level $level, "  int init();";
  println;
  print_level $level, "private:";
  print_level $level, "  /// Disallow the copy constructor.";
  print_level $level, "  $class_name(const $class_name&);";
  println;
  print_level $level, "  /// Disallow the assignment operator.";
  print_level $level, "  $class_name& operator=(const $class_name&);";
  print_level $level, "};";
  println;

  my @namespaces_end = reverse @namespaces;
  foreach my $namespace (@namespaces_end)
  {
    print_level $level, "} // end namespace $namespace";
  }

  println;
  println "#endif // ",$header_guard;
  println;
}

#############################################################################
#
sub write_cpp_file_body
{
  println "#include <$path/$class_name.h>";
  println;

  foreach my $namespace (@namespaces)
  {
    println "namespace $namespace";
    println "{";
  }

  println;
  write_method_header();
  println "$class_name\:\:$class_name()";
  println "{\n}\n";

  write_method_header();
  println "$class_name\:\:~$class_name()";
  println "{\n}\n";

  # write_method_header();
  # println "int $class_name\:\:init()";
  # println "{";
  # println "  return 0;";
  # println "}\n";

  write_method_header();
  println;

  my @namespaces_end = reverse @namespaces;
  foreach my $namespace (@namespaces_end)
  {
    println "} // end namespace $namespace";
  }

  println;
}

#############################################################################
#
sub write_test_file_body
{
  my $class_path = $path;
  $class_path =~ s/\/test//;

  println "// this is a preprocessor test to ensure that the object cannot be included";
  println "// twice.";
  println "#include <$class_path/$class_tested.h>";
  println;

  println "#include <gtest/gtest.h>";
  println "#include <gmock/gmock.h>";
  println;
  println "using \:\:testing\:\:_\; // allow the \"anything goes\" character in GMock";
  println;

  #println "#include <cps/lib/system/target/platform.h>";
  println "#include <$class_path/$class_tested.h>";
  println;

  if ( scalar @namespaces > 0 )
  {
    print {$outfile} "using namespace ", join( '::', @namespaces), ";\n";
    println;
  }

  write_method_header();
  println "TEST($class_name, Stack)";
  println "{";
  println "  // create on stack";
  println "  $class_tested object;";
  println "}";

  write_method_header();
  println "TEST($class_name, Heap)";
  println "{";
  println "  // create in the heap";
  println "  $class_tested* p_object = new $class_tested;";
  println "  delete p_object;";
  println "}";
  println;

  write_method_header();
  println;
}

#############################################################################
#
sub write_h_interface_file_body
{
  my $level = 0;
  my $header_guard = create_header_guard();

  println "#ifndef ",$header_guard;
  println "#define ",$header_guard;
  println;

  foreach my $namespace (@namespaces)
  {
    print_level $level, "namespace $namespace";
    print_level $level, "{";
  }

  println;
  print_level $level, "/// Interface class description.";
  print_level $level, "class $class_name";
  print_level $level, "{";
  print_level $level, "public:";
  print_level $level, "  /// Object destructor.";
  print_level $level, "  virtual ~$class_name() {}";
  print_level $level, "  ";
  # print_level $level, "  /// Example virtual function.";
  # print_level $level, "  ///";
  # print_level $level, "  /// \@param[in] arg - Example argument.";
  # print_level $level, "  ///";
  # print_level $level, "  /// \@return Example function return.";
  # print_level $level, "  virtual int foo(int arg) = 0;";
  print_level $level, "};";
  println;

  my @namespaces_end = reverse @namespaces;
  foreach my $namespace (@namespaces_end)
  {
    print_level $level, "} // $namespace";
  }

  println;
  println "#endif // ",$header_guard;
  println;
}

#############################################################################
#
sub write_h_mock_file_body
{
  my $level = 0;
  my $class_path = $path;
  $class_path =~ s/\/mocks//;

  my $header_guard = create_header_guard();

  println "#ifndef ",$header_guard;
  println "#define ",$header_guard;
  println;

  println "#include <$class_path/$interface_class.h>";
  println;

  foreach my $namespace (@namespaces)
  {
    print_level $level, "namespace $namespace";
    print_level $level, "{";
  }

  println;
  print_level $level, "/// Mock class description.";
  print_level $level, "class $class_name : public $interface_class";
  print_level $level, "{";
  print_level $level, "public:";
  print_level $level, "  ";
  print_level $level, "};";

  my @namespaces_end = reverse @namespaces;
  foreach my $namespace (@namespaces_end)
  {
    print_level $level, "} // $namespace";
  }

  println;
  println "#endif // ",$header_guard;
  println;
}

#############################################################################
#
sub write_test_file
{
  my $file = "$class_name.cpp";
  print "Creating $file\n";

  $class_tested = $class_name;
  $class_tested =~ s/_test//;

  open $outfile, '>', $file or die "Can't open $file";
  write_test_file_header();
  write_test_file_body();
  close $outfile;
}

#############################################################################
#
sub write_cpp_file
{
  my $file = "$class_name.cpp";
  print "Creating $file\n";

  open $outfile, '>', $file or die "Can't open $file";
  write_cpp_file_header();
  write_cpp_file_body();
  close $outfile;
}

#############################################################################
#
sub write_h_file
{
  my $file = "$class_name.h";
  print "Creating $file\n";

  open $outfile, '>', $file or die "Can't open $file";
  write_h_file_header();
  write_h_file_body();
  close $outfile;
}

#############################################################################
#
sub write_h_interface_file
{
  my $file = "$class_name.h";
  print "Creating interface $file\n";

  open $outfile, '>', $file or die "Can't open $file";
  write_h_file_header();
  write_h_interface_file_body();
  close $outfile;
}

#############################################################################
#
sub write_h_mock_file
{
  my $file = "$class_name.h";
  print "Creating mock $file\n";

  open $outfile, '>', $file or die "Can't open $file";

  $interface_class = $class_name;
  $interface_class =~ s/Mock_/I_/;

  write_h_file_header();
  write_h_mock_file_body();
  close $outfile;
}

#############################################################################
#
main(@ARGV);
