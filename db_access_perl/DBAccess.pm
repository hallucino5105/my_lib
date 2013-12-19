package DBAccess;


use strict;
use warnings;
use DBI;
use XML::TreePP;
use Data::Dumper;


sub new
{
	my $class = shift;
	my ($host, $user, $pass, $database) = @_;

	my $this = {
		host       => $host,
		user       => $user,
		pass       => $pass,
		database   => $database,
		dbh        => undef,
		sth        => undef,
		lastResult => undef
	};

	$this->{dbh} = DBI->connect(
        "dbi:mysql:host=$this->{host};database=$this->{database};",
        $this->{user},
        $this->{pass},
        { RaiseError => 0 , AutoCommit => 1 })
        or die $DBI::errstr;

	return bless($this, $class);
}


sub DESTROY
{
	my $this = shift;

    #$this->{dbh}->disconnect;
}


sub resultHash()
{
	my $this = shift;

	if(!defined($this->{lastResult})) { die "no def lastResult." };

	return $this->{lastResult};
}


sub resultStr()
{
	my $this = shift;
	my $tree = XML::TreePP->new(
        output_encoding => "UTF-8",
        indent => 0
        );

	if(!defined($this->{lastResult})) { die "no def lastResult." };

	return $tree->write($this->{lastResult});
}


sub characterCode()
{
	my $this = shift;
	my ($cc) = @_;

	if(!defined($this->{dbh})) { die "no def dbs." };

	$this->{dbh}->do("set names $cc");
}


sub commit()
{
	my $this = shift;
	my @columns = @_;

	if(!defined($this->{sth})) { die "no def sth." };

	$this->{lastResult} = { list => { item => [] } };
	while(my $item = $this->{sth}->fetchrow_hashref) {
		push(@{$this->{lastResult}->{list}->{item}}, $item);
	}

	$this->{sth}->finish;
}


sub select()
{
	my $this = shift;
	my ($query, @args) = @_;

	if(!defined($this->{dbh})) { die "no def dbs." };

	$this->{sth} = $this->{dbh}->prepare($query);
	$this->{sth}->execute(@args);

	&commit($this);
}


sub query()
{
	my $this = shift;
	my ($query, @args) = @_;

	if(!defined($this->{dbh})) { die "no def dbs." };

	$this->{sth} = $this->{dbh}->prepare($query);
	$this->{sth}->execute(@args);
	$this->{sth}->finish;
}


sub insert()
{
	my $this = shift;
	my ($query, @args) = @_;

	&query($this, $query, @args);
}


sub create()
{
	my $this = shift;
	my ($table_name, $query, @args) = @_;

	if(!isExistTable($this, $table_name)) {
		&query($this, $query, @args);
	}
}


sub isExistTable()
{
	my $this = shift;
	my $table_name = shift;

	return $this->{dbh}->tables('', '', $table_name);
}


1;
